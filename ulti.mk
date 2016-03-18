# Release name
PRODUCT_RELEASE_NAME := GT-I8160

# Boot animation
TARGET_SCREEN_HEIGHT := 800
TARGET_SCREEN_WIDTH := 480

# Inherit some common AOSP stuff.
#$(call inherit-product, vendor/aosp/config/common.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/codina/ulti_codina.mk)

# Device identifier. This must come after all inclusions
PRODUCT_DEVICE := codina
PRODUCT_NAME := ulti_codina
PRODUCT_BRAND := samsung
PRODUCT_MODEL := GT-I8160
PRODUCT_MANUFACTURER := samsung

# Set build fingerprint / ID / Product Name ect.
PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=GT-I8160 TARGET_DEVICE=codina
