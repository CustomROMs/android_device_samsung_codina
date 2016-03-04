LOCAL_PATH := device/samsung/codina

# DEVICE_ENABLE_SELINUX := true
# DEVICE_SDCARD_ON_SWIPE := true

# TWRP
# TW_THEME := portrait_mdpi
# TARGET_RECOVERY_FORCE_PIXEL_FORMAT := RGB_565
# RECOVERY_GRAPHICS_FORCE_USE_LINELENGTH := true
DEVICE_RESOLUTION := 480x800
TW_INTERNAL_STORAGE_PATH := "/storage/sdcard0"
TW_INTERNAL_STORAGE_MOUNT_POINT := "sdcard0"
TW_EXTERNAL_STORAGE_PATH := "/storage/sdcard1"
TW_EXTERNAL_STORAGE_MOUNT_POINT := "sdcard1"
TW_BOARD_CUSTOM_GRAPHICS := ../../../device/samsung/codina/recovery/twrp-graphics.c
TW_HAS_NO_RECOVERY_PARTITION := true
TW_FLASH_FROM_STORAGE := true
TW_INCLUDE_FUSE_EXFAT := true
TW_BRIGHTNESS_PATH := /sys/class/backlight/panel/brightness
TW_MAX_BRIGHTNESS := 255
TW_NO_USB_STORAGE := true
TW_NO_REBOOT_BOOTLOADER := true
TW_HAS_DOWNLOAD_MODE := true

# RECOVERY_SDCARD_ON_DATA := true
# TW_INCLUDE_CRYPTO := true
#TW_NO_EXFAT := true
#TW_NO_EXFAT_FUSE := true

ifeq ($(DEVICE_ENABLE_SELINUX),true)
# TWRP flags
TWHAVE_SELINUX := true
TW_INCLUDE_JB_CRYPTO := true
TW_CRYPTO_FS_TYPE := "ext4"
TW_CRYPTO_REAL_BLKDEV := "/dev/block/mmcblk0p5"
TW_CRYPTO_MNT_POINT := "/data"
TW_CRYPTO_FS_OPTIONS := "noatime,nosuid,nodev,discard,noauto_da_alloc,journal_async_commit,errors=panic wait,check"
TW_CRYPTO_FS_FLAGS := "0x00000406"
TW_CRYPTO_KEY_LOC := "/efs/metadata"
endif

# Init files
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/device_tunables.rc:root/device_tunables.rc \
    $(LOCAL_PATH)/rootdir/init.kernel.rc:root/init.kernel.rc \
    $(LOCAL_PATH)/rootdirnew/init.samsungcodina.rc:root/init.samsungcodina.rc \
    $(LOCAL_PATH)/rootdir/init.recovery.samsungcodina.rc:root/init.recovery.samsungcodina.rc \
    $(LOCAL_PATH)/rootdir/ueventd.samsungcodina.rc:root/ueventd.samsungcodina.rc \
    $(LOCAL_PATH)/rootdir/init.samsungcodina.usb.rc:root/init.samsungcodina.usb.rc

ifeq ($(DEVICE_SDCARD_ON_SWIPE),true)
# fstab
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdirnew/fstab.samsungcodina:root/fstab.samsungcodina
else
# fstab codina
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/fstab.samsungcodina:root/fstab.samsungcodina
endif

# TWRP
PRODUCT_COPY_FILES += $(LOCAL_PATH)/recovery/twrp.fstab:recovery/root/etc/twrp.fstab
