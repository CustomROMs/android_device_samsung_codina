LOCAL_PATH := $(call my-dir)

# camera

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
     VectorImpl.c

LOCAL_SHARED_LIBRARIES := liblog libcutils libgui libbinder libutils
LOCAL_MODULE := libshim_audio
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
