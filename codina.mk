#
# Copyright (C) 2012 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := device/samsung/codina

DEVICE_PACKAGE_OVERLAYS := $(LOCAL_PATH)/overlay

# System properties
-include $(LOCAL_PATH)/system_prop.mk

# Our devices are HDPI
PRODUCT_AAPT_CONFIG := normal hdpi
PRODUCT_AAPT_PREF_CONFIG := hdpi

# new google video codecs for low end devices
DEVICE_ENABLE_LOV := true
DEVICE_WiFi_NEW := true
# DEVICE_ENABLE_SUBMIX := true
DEVICE_ENABLE_CHANGER_CM := true
DEVICE_AUDIO_MOD := true
# Sensors modules HAL
# DEVICE_SENSOR_MULTI_HAL := true

# Media
ifeq ($(DEVICE_ENABLE_LOV),true)
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/omxloaders:system/etc/omxloaders \
    $(LOCAL_PATH)/configsnew/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/configsnew/media_profiles.xml:system/etc/media_profiles.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video_le.xml:system/etc/media_codecs_google_video_le.xml
else
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/omxloaders:system/etc/omxloaders \
    $(LOCAL_PATH)/configs/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/configs/media_profiles.xml:system/etc/media_profiles.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml
endif

# Wifi
ifeq ($(DEVICE_WiFi_NEW),true)
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configsnew/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
    $(LOCAL_PATH)/configsnew/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf
else
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf

$(call inherit-product, hardware/broadcom/wlan/bcmdhd/config/config-bcm.mk)
endif

PRODUCT_PACKAGES += \
    libnetcmdiface \
    wpa_supplicant \
    wpa_supplicant.conf

# Wi-Fi firmware
# $(call inherit-product-if-exists, hardware/broadcom/wlan/bcmdhd/firmware/bcm4330/device-bcm.mk)

# Wi-Fi test
PRODUCT_PACKAGES += \
    libwpa_client \
    hostapd

#PRODUCT_PACKAGES += \
#    dhcpcd.conf
#    hostapd_default.conf

# Bluetooth
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf

# STE
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/cspsa.conf:system/etc/cspsa.conf \
    $(LOCAL_PATH)/configs/usbid_init.sh:system/bin/usbid_init.sh

# Audio
ifeq ($(DEVICE_ENABLE_SUBMIX),true)
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configsnew/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/configs/asound.conf:system/etc/asound.conf

PRODUCT_PACKAGES += \
    audio.r_submix.default

else
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/configs/asound.conf:system/etc/asound.conf
endif

PRODUCT_PACKAGES += \
    audio.usb.default \
    audio.a2dp.default \
    libaudioutils \
    libtinyalsa

# U8500 Hardware
$(call inherit-product, hardware/u8500/u8500.mk)

# Charger
ifeq ($(DEVICE_ENABLE_CHANGER_CM),true)
PRODUCT_PACKAGES += \
    charger_res_images
else
# Charger
# Charger Prebuilt (temporary solution for lollipop)
# Use prebuilt charger and images from KitKat
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/prebuilt/charger/charger:root/sbin/charger \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_0.png:root/res/images/charger/battery_0.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_1.png:root/res/images/charger/battery_1.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_2.png:root/res/images/charger/battery_2.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_3.png:root/res/images/charger/battery_3.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_4.png:root/res/images/charger/battery_4.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_5.png:root/res/images/charger/battery_5.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_charge.png:root/res/images/charger/battery_charge.png \
    $(LOCAL_PATH)/prebuilt/charger/images/battery_fail.png:root/res/images/charger/battery_fail.png
endif

# Misc Packages
PRODUCT_PACKAGES += \
    com.android.future.usb.accessory \
    SamsungServiceMode \
    Stk

# Filesystem management
PRODUCT_PACKAGES += \
    make_ext4fs \
    setup_fs

# F2FS
PRODUCT_PACKAGES += \
    mkfs.f2fs \
    fsck.f2fs \
    fibmap.f2fs

# Keylayout
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    $(LOCAL_PATH)/configs/keylayout/sec_touchkey.kl:system/usr/keylayout/sec_touchkey.kl \
    $(LOCAL_PATH)/configs/keylayout/simple_remote.kl:system/usr/keylayout/simple_remote.kl

# These are the hardware-specific features
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.location.xml:system/etc/permissions/android.hardware.location.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.software.sip.xml:system/etc/permissions/android.software.sip.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml

# These are the hardware-audio low
#PRODUCT_COPY_FILES += \
#    frameworks/native/data/etc/android.hardware.audio.low_latency.xml:system/etc/permissions/android.hardware.audio.low_latency.xml

# Live Wallpapers
PRODUCT_PACKAGES += \
    librs_jni

# Libstport
PRODUCT_PACKAGES += \
    libstlport

# Busybox
PRODUCT_PACKAGES += \
    Busybox

# Snap Camera
PRODUCT_PACKAGES += \
    Snap

ifeq ($(DEVICE_AUDIO_MOD),true)
# MusicFX
PRODUCT_PACKAGES += \
    MusicFX
else
# AudioFX
PRODUCT_PACKAGES += \
    AudioFX
endif

# HIDL
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configsnew/manifest.xml:system/vendor/manifest.xml

# Bluetooth
PRODUCT_PACKAGES += \
    libbt-vendor

# Bluetooth HAL
PRODUCT_PACKAGES += \
    android.hardware.bluetooth@1.0-impl

# wificond
PRODUCT_PACKAGES += \
    wificond

# Wifi
PRODUCT_PACKAGES += \
    android.hardware.wifi@1.0-service

# USB HAL
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.0-impl

# vibrato
PRODUCT_PACKAGES += \
    android.hardware.vibrator@1.0-impl

# GNSS HAL
PRODUCT_PACKAGES += \
    android.hardware.gnss@1.0-impl

# RenderScript HAL
PRODUCT_PACKAGES += \
    android.hardware.renderscript@1.0-impl

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl

# graphics HIDL interfaces
PRODUCT_PACKAGES += \
    android.hardware.configstore@1.0-service \
    android.hardware.graphics.allocator@2.0-impl-exynos4 \
    android.hardware.graphics.mapper@2.0-impl-exynos4 \
    android.hardware.graphics.composer@2.1-impl


# Keymaster
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl

# Camera legacy
PRODUCT_PACKAGES += \
    android.hardware.camera.provider@2.4-impl-legacy \
    camera.device@1.0-impl-legacy

# Camera
#PRODUCT_PACKAGES += \
#    android.hardware.camera.provider@2.4-impl \
#    camera.device@1.0-impl

# health
PRODUCT_PACKAGES += \
    android.hardware.health@1.0-impl

# Lights
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-impl

# memtrack
PRODUCT_PACKAGES += \
    android.hardware.memtrack@1.0-impl

PRODUCT_PACKAGES += \
    android.hardware.sensors@1.0-impl

PRODUCT_PACKAGES += \
    input.evdev.default

# Gatekeeper HAL
#PRODUCT_PACKAGES += \
#    android.hardware.gatekeeper@1.0-service \
#    android.hardware.gatekeeper@1.0-impl

# Audio
#PRODUCT_PACKAGES += \
#    android.hardware.audio@2.0-impl \
#    android.hardware.audio.effect@2.0-impl \
#    android.hardware.broadcastradio@1.0-impl
#    android.hardware.soundtrigger@2.0-impl

# Selective SPN list for operator number who has the problem.
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/selective-spn-conf.xml:system/etc/selective-spn-conf.xml

# Selective power profile from a file.
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/power_profile.xml:system/etc/power_profile.xml

# Screencast
#PRODUCT_PACKAGES += \
#    Screencast

PRODUCT_PACKAGES += \
    su

# Permissions
PRODUCT_PACKAGES += \
    privapp-permissions-microg.xml

# We have enough storage space to hold precise GC data
PRODUCT_TAGS += dalvik.gc.type-precise

# Use the non-open-source parts, if they're present
include vendor/samsung/u8500-common/vendor-common.mk

# == BEGIN LOCAL CONFIG ==

# For better compatibility with ROMs (like Slim, PAC)
$(call inherit-product, vendor/samsung/u8500-common/codina/codina-vendor-blobs.mk)

# Build GO
$(call inherit-product, build/make/target/product/go_defaults_512.mk)

# STE Modem
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/ste_modem.sh:system/etc/ste_modem.sh

# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/adm.sqlite-u8500:system/etc/adm.sqlite-u8500

# GPS
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/gps.conf:system/etc/gps.conf \
    $(LOCAL_PATH)/configs/sirfgps.conf:system/etc/sirfgps.conf
