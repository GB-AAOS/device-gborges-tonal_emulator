# Tonal Emulator AOSP Device

This repository provides the `tonal_emulator` device configuration for AOSP. It includes a custom virtual proximity sensor and a monitoring application integrated into the system build.

## Project Overview

The `tonal_emulator` demonstrates a complete AOSP stack integration. It features:
* **Fake Proximity Sensor**: A C++ Sensor Sub-HAL implementation that generates sinusoidal values. Built using the Sensors Multi-HAL 2.1 framework.
* **Proximity Monitor App**: A pre-installed system application that displays and logs sensor data in real-time.
* **Property Override**: A developer hook to manually control sensor output via system properties.
* **SEPolicy**: Treble-compliant security policies ensuring secure communication between the system framework and the vendor HAL.

## Setup and Build Instructions

Follow these steps to initialize the environment and build the `tonal_emulator` image.

### 1. Initialize AOSP Repository
Initialize the AOSP tree using the Android 15 r20 tag:

~~~bash
repo init -u https://android.googlesource.com/platform/manifest -b android-15.0.0_r20
repo sync -j$(nproc)
~~~

### 2. Clone Device Configuration
Clone this repository into the specific device directory:

~~~bash
mkdir -p device/gborges/tonal_emulator
# Clone the repository into this folder
git clone https://github.com/GB-AAOS/device-gborges-tonal_emulator device/gborges/tonal_emulator
~~~

### 3. Build the Image
Source the build environment and select the `tonal_emulator` target:

~~~bash
source build/envsetup.sh
lunch tonal_emulator-trunk_staging-userdebug
m
~~~

### 4. Run the Emulator
Launch the tonal emulator:

~~~bash
emulator
~~~

## Features and Verification

### Fake Proximity Sensor
The sensor is integrated via the Sensors Multi-HAL. It produces a sinusoidal wave (0.5 + 0.5 * sin(t)) every 500ms, running at a frequency of 0.1Hz.

To verify the sensor registration:
~~~bash
adb shell dumpsys sensorservice | grep -A 10 "Proximity Fake Sensor"
~~~

### Property Override
You can bypass the sinusoidal logic and force a specific sensor value via shell. This is useful for testing edge cases in the framework or apps.

~~~bash
# Force the sensor to output 0.7
adb shell setprop vendor.proximity.override 0.7

# Return to automatic sinusoidal mode
adb shell setprop vendor.proximity.override -1.0
~~~

### Proximity Monitor App
The integrated monitoring application displays the current sensor value and calculates the HAL-to-App latency in milliseconds. It also throttles telemetry to `logcat` once every second.

~~~bash
adb logcat -s ProximityMonitorApp
~~~

## Architecture and Security

### SELinux Implementation
The device includes custom SEPolicy rules to allow the `system_server` to connect to the vendor sensor HAL. It defines the `vendor_proximity_prop` type, allowing the HAL to read the override property while strictly adhering to Treble partition boundaries and VTS namespace requirements.

## Documentation & Resources

* **Design Document**: Detailed architectural decisions and SELinux debugging steps can be found in [docs/design_document.md](docs/design_document.md).
* **Demo Video**: [View the working build and property override demo](https://drive.google.com/file/d/1vFgJzZ5D2TgMu3AuKxU12QQ6zRDGFWW-/view?usp=sharing).