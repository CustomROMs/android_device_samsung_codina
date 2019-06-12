LOCAL_PATH := $(call my-dir)

# audio libs

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	SharedBuffer.cpp \
	VectorImpl.cpp

LOCAL_C_INCLUDES += external/safe-iop/include
LOCAL_SHARED_LIBRARIES := liblog libutils
LOCAL_MODULE := libLog
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

include $(call first-makefiles-under,$(LOCAL_PATH))
