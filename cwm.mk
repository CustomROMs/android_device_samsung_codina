LOCAL_PATH := device/samsung/codina

# DEVICE_SDCARD_ON_SWIPE := true

# CyanogenMod CWM
BOARD_RECOVERY_NEEDS_REL_INPUT := true
# TARGET_RECOVERY_DENSITY := hdpi
# BOARD_HAS_DOWNLOAD_MODE := true
RECOVERY_FSTAB_VERSION := 2

# Init files
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/device_tunables.rc:root/device_tunables.rc \
    $(LOCAL_PATH)/rootdir/init.kernel.rc:root/init.kernel.rc \
    $(LOCAL_PATH)/rootdir/init.recovery.samsungcodina.rc:root/init.recovery.samsungcodina.rc \
    $(LOCAL_PATH)/rootdirnew/init.samsungcodina.rc:root/init.samsungcodina.rc \
    $(LOCAL_PATH)/rootdir/init.samsungcodina.usb.rc:root/init.samsungcodina.usb.rc \
    $(LOCAL_PATH)/rootdirnew/ueventd.samsungcodina.rc:root/ueventd.samsungcodina.rc

# fstab
ifeq ($(DEVICE_SDCARD_ON_SWIPE),true)
# fstab swipe
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdirnew/fstab.samsungcodina:root/fstab.samsungcodina
else
# fstab codina
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/fstab.samsungcodina:root/fstab.samsungcodina
endif
