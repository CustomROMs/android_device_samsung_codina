LOCAL_PATH := $(call my-dir)

#
# compat symbols for gps
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES := libshim_gps.c
LOCAL_SHARED_LIBRARIES := liblog libutils libgui
LOCAL_MODULE := libGui
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
