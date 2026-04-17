EMULATOR_VENDOR_NO_SENSORS := true
PRODUCT_COPY_FILES += \
    device/gborges/emulator/sensors/hals.conf:$(TARGET_COPY_OUT_VENDOR)/etc/sensors/hals.conf


$(call inherit-product, device/generic/goldfish/64bitonly/product/sdk_phone64_x86_64.mk)

# Product properties
PRODUCT_NAME := emulator
PRODUCT_DEVICE := emu64x
PRODUCT_BRAND := Android
PRODUCT_MODEL := Android SDK x86_64 emulator for Tonal Challenge

DEVICE_MANIFEST_FILE := device/generic/goldfish/manifest.xml
BOARD_VENDOR_SEPOLICY_DIRS += device/gborges/emulator/sepolicy

# Tonal Challenge properties
PRODUCT_PACKAGES += \
    android.hardware.sensors@2.1-service.multihal \
    android.hardware.sensors@2.1-subhal.gborges

PRODUCT_PACKAGES += ProximityMonitorApp

