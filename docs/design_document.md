# Design Document: Tonal Challenge

## 1. System Architecture

### 1.1 The Sub-HAL (Vendor Partition)
The sensor logic is implemented as a 2.1 Sub-HAL library. It’s a passthrough implementation loaded by the `android.hardware.sensors@2.1-service.multihal` binary. 
* **Worker Thread:** A dedicated thread handles the "hardware" simulation, using a fixed 0.1Hz frequency anchored to the system monotonic clock (`android::elapsedRealtimeNano()`) to generate the sensor signal (sine wave). This prevents drift and ensures data consistency regardless of CPU load or scheduling jitter.
* **Heartbeat:** The thread samples the state every **500ms**.

### 1.2 Monitor App & Latency Tracking
The `ProximityMonitorApp` is a standard system application that acts as the primary consumer for the sensor data.
* **Interface:** Uses the `SensorManager` API to register a listener for `Sensor.TYPE_PROXIMITY`.
* **Throttling:** While the HAL generates data at 2Hz (500ms), the app throttles logging to 1s intervals to keep logcat readable while still proving the sensor is alive.
* **Latency Reader:** To verify system performance, the app calculates the delta between the hardware timestamp (`event.timestamp`) generated in the HAL and the current `SystemClock.elapsedRealtimeNanos()` when the event hits `onSensorChanged`. This provides a real-time measurement of the HAL-to-App latency, confirming the stack stays well within the required limits (<1 second).

---

## 2. Property Override Feature
The stack supports a **Dynamic Override** via system properties to simulate external hardware triggers or manual debugging.

* **Mechanism:** The HAL polls `vendor.proximity.override` during every 500ms loop using the `android::base` property headers.
* **Logic:** If the property contains a valid float (0.0 to 1.0), the sinusoidal logic is bypassed.
* **Refection Time:** With the HAL polling at 500ms and the app's latency tracking, any `setprop` change is reflected in the logs in under 1s, satisfying the maximum latency constraint.

---

## 3. SELinux & Treble Integration
Navigating the Treble isolation between the System and Vendor partitions was the main hurdle in getting the sensor to register properly.

### 3.1 The Violations (Audit Logs)
In `Enforcing` mode, the sensor was initially blocked from registration and the property read failed. These logs were pulled from `dmesg`:

~~~text
# Binder Registration Block
avc: denied { find } for interface=android.hardware.sensors@2.1::ISensors scontext=u:r:system_server:s0 tcontext=u:r:hal_sensors_default:s0

# Build-time Neverallow Violation
libsepol.report_failure: neverallow on line 158 of system/sepolicy/private/property.te 
violated by allow hal_sensors_default vendor_proximity_prop:file { read open };

# VTS Namespace Violation
vendor property_contexts MUST use ONLY vendor-prefixed properties.
Violation: my.proximity.override u:object_r:vendor_proximity_prop:s0
~~~

### 3.2 Root Cause Analysis
1.  **Binder Isolation:** The `system_server` (Core) didn't have permission to "find" the `hal_sensors_default` (Vendor) in the hardware service manager.
2.  **Namespace Rules:** Using a generic property name violated Treble boundaries. AOSP forbids Vendor HALs from reading properties that lack the `vendor.` prefix or the `vendor_property_type` attribute to keep the partitions independent.

### 3.3 The Fix
The policy was hardened using Treble-compliant rules rather than disabling SELinux:
1.  **Prefixing:** Renamed the property to `vendor.proximity.override`.
2.  **Context Mapping:** Added to `device/gborges/tonal_emulator/sepolicy/property_contexts`:
    ~~~
    vendor.proximity.override u:object_r:vendor_proximity_prop:s0
    ~~~
3.  **Policy Attribution:** Defined the property as a `vendor_property_type` in `hal_sensors_gborges.te` and granted `get_prop` permissions to the HAL. This satisfies the "neverallow" assertions while allowing the cross-partition read.

---

## 4. Final Verification
Using the Multi-HAL architecture allowed for a modular integration that doesn't interfere with other emulator sensors. The setup is fully VTS-compliant and stable in `Enforcing` mode, with the monitor app confirming consistent 500ms data delivery and low-latency property overrides.

### 4.1 Demo
A demonstration of the working AOSP build, including sensor registration and the property override mechanism, can be viewed here:
[Tonal Challenge Demo](https://drive.google.com/file/d/1vFgJzZ5D2TgMu3AuKxU12QQ6zRDGFWW-/view?usp=sharing)