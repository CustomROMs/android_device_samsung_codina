# Copyright (C) 2008 The Android Open Source Project
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

LOCAL_PATH:= $(call my-dir)

commonSources:= Flattenable.cpp

# For the device, static
# =====================================================
include $(CLEAR_VARS)


# we have the common sources, plus some device-specific stuff
LOCAL_SRC_FILES:= \
	$(commonSources)

LOCAL_STATIC_LIBRARIES := \
	libcutils \
	libc

LOCAL_SHARED_LIBRARIES := \
        libbacktrace \
        libLog \
        libdl

LOCAL_MODULE:= libUtils
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += external/safe-iop/include
include $(BUILD_STATIC_LIBRARY)

# For the device, shared
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE:= libUtils
LOCAL_WHOLE_STATIC_LIBRARIES := libUtils
LOCAL_SHARED_LIBRARIES := \
        libbacktrace \
        libcutils \
        libdl \
        libLog \
        libutils

LOCAL_CFLAGS := -Werror
LOCAL_C_INCLUDES += external/safe-iop/include

include $(BUILD_SHARED_LIBRARY)
