# Tonal Emulator AOSP Device

This repository provides the `tonal_emulator` device configuration for AOSP. It includes a custom virtual proximity sensor and a monitoring application integrated into the system build.

## Project Overview

The `tonal_emulator` is designed to demonstrate full-stack Android integration. It features:
* **Fake Proximity Sensor**: A C++ Sensor Sub-HAL implementation that generates sinusoidal values. Uses sensor multihal 2.1.
* **Proximity Monitor App**: A pre-installed system application that tracks and logs sensor data.
* **Property Override**: A mechanism to manually control sensor output via system properties.
* **SEPolicy**: Treble-compliant security policies ensuring proper communication between the framework and the vendor HAL.

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

### 4. Run the emulator
Run the tonal emulator:

~~~bash
emulator
~~~

## Features and Verification

### Fake Proximity Sensor
The sensor is integrated via the Sensors Multi-HAL. It produces a sinusoidal wave (0.5 + 0.5 * sin(t)) every 500ms, at a frequency of 0.1Hz.

To verify the sensor is running:
~~~bash
adb shell dumpsys sensorservice | grep -A 10 "Proximity Fake Sensor"
~~~

### Property Override
You can bypass the sinusoidal logic and force a specific sensor value using a system property. This is useful for testing specific application states.

~~~bash
# Force the sensor to output 0.7
adb shell setprop vendor.proximity.override 0.7

# Return to automatic sinusoidal mode
adb shell setprop vendor.proximity.override -1.0
~~~

### Proximity Monitor App
The built-in monitoring application shows the current value from the sensor and the latency in milliseconds, it also logs the current value to `logcat` every second.

~~~bash
adb logcat -s ProximityMonitorApp
~~~

## Architecture and Security

### SELinux Implementation
The device includes custom SEPolicy rules to allow the `system_server` to communicate with the vendor sensor HAL. It also defines the `vendor_proximity_prop` type, allowing the HAL to read the override property.