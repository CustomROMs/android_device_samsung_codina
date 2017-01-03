LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := main.cpp PowerHAL.cpp
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libpowermanager \
    liblog \
    libbinder \
    libutils

LOCAL_MODULE := profile

include $(BUILD_EXECUTABLE)

