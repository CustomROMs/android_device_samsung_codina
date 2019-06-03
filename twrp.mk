LOCAL_PATH := device/samsung/codina

# DEVICE_SDCARD_ON_SWIPE := true
DEVICE_ENABLE_RECOVERY_3 := true
DEVICE_WLAN_ON_NEW := true
DEVICE_UMS_ON_NEW := true
DEVICE_RECOVERY_CRYPTO := true

# TWRP
# TW_THEME := portrait_mdpi
DEVICE_RESOLUTION := 480x800
COMMON_GLOBAL_CFLAGS += -DTW_NO_SCREEN_BLANK
TW_EXCLUDE_PARTED := true
ifeq ($(DEVICE_ENABLE_RECOVERY_3),true)
TARGET_RECOVERY_PIXEL_FORMAT := "RGB_565"
TARGET_RECOVERY_FORCE_PIXEL_FORMAT := "RGB_565"
RECOVERY_GRAPHICS_FORCE_USE_LINELENGTH := true
else
TW_BOARD_CUSTOM_GRAPHICS := ../../../device/samsung/codina/recovery/twrp-graphics.c
endif
TW_INTERNAL_STORAGE_PATH := "/storage/sdcard0"
TW_INTERNAL_STORAGE_MOUNT_POINT := "sdcard0"
TW_EXTERNAL_STORAGE_PATH := "/storage/sdcard1"
TW_EXTERNAL_STORAGE_MOUNT_POINT := "sdcard1"
TW_HAS_NO_RECOVERY_PARTITION := true
TW_FLASH_FROM_STORAGE := true
TW_INCLUDE_FUSE_EXFAT := true
TW_BRIGHTNESS_PATH := /sys/class/backlight/panel/brightness
TW_MAX_BRIGHTNESS := 255
TW_NO_USB_STORAGE := true
TW_NO_REBOOT_BOOTLOADER := true
TW_HAS_DOWNLOAD_MODE := true
# RECOVERY_SDCARD_ON_DATA := true
TW_EXCLUDE_SUPERSU := true
# TW_USE_TOOLBOX := true

ifeq ($(DEVICE_RECOVERY_CRYPTO),true)
# TWRP flags
# TWHAVE_SELINUX := true
TW_INCLUDE_JB_CRYPTO := true
TW_CRYPTO_FS_TYPE := "ext4"
TW_CRYPTO_REAL_BLKDEV := "/dev/block/mmcblk0p5"
TW_CRYPTO_MNT_POINT := "/data"
TW_CRYPTO_FS_OPTIONS := "noatime,nosuid,nodev,noauto_da_alloc,journal_async_commit,errors=panic wait,check"
TW_CRYPTO_FS_FLAGS := "0x00000406"
TW_CRYPTO_KEY_LOC := "/efs/metadata"
endif

# TWRP
PRODUCT_COPY_FILES += $(LOCAL_PATH)/recovery/twrp.fstab:recovery/root/etc/twrp.fstab
