$(call inherit-product, device/generic/goldfish/64bitonly/product/sdk_phone64_x86_64.mk)

# Product properties
PRODUCT_NAME := emulator
PRODUCT_DEVICE := emu64x
PRODUCT_BRAND := Android
PRODUCT_MODEL := Android SDK x86_64 emulator for Tonal Challenge

# Tonal Challenge properties
PRODUCT_PACKAGES += \
    android.hardware.sensors-service.multihal \
    android.hardware.sensors@2.1-impl.gborges

PRODUCT_COPY_FILES += \
    device/gborges/emulator/sensors/hals.conf:$(TARGET_COPY_OUT_VENDOR)/etc/sensors/hals.conf
