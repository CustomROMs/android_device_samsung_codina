# Copyright (C) 2009 The Android Open Source Project
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

# we have the common sources, plus some device-specific stuff
sources := \
    Parcel.cpp \
    MemoryBase.cpp

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libBinder
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../libUtils/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES += libLog libcutils libutils libbinder
LOCAL_SRC_FILES := $(sources)
ifneq ($(TARGET_USES_64_BIT_BINDER),true)
ifneq ($(TARGET_IS_64_BIT),true)
LOCAL_CFLAGS += -DBINDER_IPC_32BIT=1
endif
endif
LOCAL_CFLAGS += -Werror
include $(BUILD_SHARED_LIBRARY)