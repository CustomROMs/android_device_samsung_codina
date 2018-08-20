LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sensors.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/janice/system/lib/hw
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_CLANG := false
LOCAL_CFLAGS := -DTAG="Sensors" -DSKIP_CXXRTOMB
LOCAL_SRC_FILES := sensors.c

include $(BUILD_SHARED_LIBRARY)
