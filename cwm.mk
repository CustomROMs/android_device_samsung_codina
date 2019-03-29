LOCAL_PATH := device/samsung/codina

DEVICE_DISABLE_F2FS := true

# CyanogenMod CWM
# BOARD_CUSTOM_GRAPHICS := ../../../device/samsung/codina/recovery/graphics.cpp
RECOVERY_FSTAB_VERSION := 2

# Init files
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/device_tunables.rc:root/device_tunables.rc \
    $(LOCAL_PATH)/rootdir/init.kernel.rc:root/init.kernel.rc \
    $(LOCAL_PATH)/rootdir/init.samsungcodina.rc:root/init.samsungcodina.rc \
    $(LOCAL_PATH)/rootdirnew/init.recovery.samsungcodina.rc:root/init.recovery.samsungcodina.rc \
    $(LOCAL_PATH)/rootdir/ueventd.samsungcodina.rc:root/ueventd.samsungcodina.rc \
    $(LOCAL_PATH)/rootdir/init.samsungcodina.usb.rc:root/init.samsungcodina.usb.rc

ifeq ($(DEVICE_DISABLE_F2FS),true)
# fstab
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/fstab.samsungcodina:root/fstab.samsungcodina
endif
