# Release name
PRODUCT_RELEASE_NAME := GT-I8160

# Boot animation
TARGET_SCREEN_HEIGHT := 800
TARGET_SCREEN_WIDTH := 480

# Inherit from those products. Most specific first.
$(call inherit-product, build/make/target/product/full_base_telephony.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/codina/codina.mk)

# Device identifier. This must come after all inclusions
PRODUCT_NAME := lineage_codina
PRODUCT_DEVICE := codina
PRODUCT_BRAND := samsung
PRODUCT_MODEL := GT-I8160
PRODUCT_MANUFACTURER := samsung
#PRODUCT_RESTRICT_VENDOR_FILES := false

# Set build fingerprint / ID / Product Name ect.
PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=GT-I8160 TARGET_DEVICE=codina
