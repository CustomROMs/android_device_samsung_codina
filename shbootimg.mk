#
# Copyright (C) 2014 NovaFusion https://github.com/NovaFusion
# Copyright (C) 2012 The CyanogenMod Project
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

LOCAL_PATH := $(call my-dir)

INSTALLED_BOOTIMAGE_TARGET := $(PRODUCT_OUT)/boot.img

uncompressed_ramdisk := $(PRODUCT_OUT)/ramdisk.cpio
$(uncompressed_ramdisk): $(INSTALLED_RAMDISK_TARGET)
	zcat $< > $@

$(PRODUCT_OUT)/recovery.cpio.gz: $(recovery_uncompressed_ramdisk)
	mkdir -p $(PRODUCT_OUT)/recovery/root/res/images/charger
	cp $(LOCAL_PATH)/prebuilt/charger.sh $(PRODUCT_OUT)/recovery/root/charger.sh
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_0.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_0.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_1.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_1.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_2.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_2.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_3.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_3.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_4.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_4.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_5.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_5.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_charge.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_charge.png
	cp $(LOCAL_PATH)/prebuilt/charger/images/battery_fail.png $(PRODUCT_OUT)/recovery/root/res/images/charger/battery_fail.png
	mkdir -p $(PRODUCT_OUT)/u8500_initramfs_files
	cp $(LOCAL_PATH)/prebuilt/u8500_initramfs_files/busybox $(PRODUCT_OUT)/u8500_initramfs_files/busybox
	cp $(LOCAL_PATH)/prebuilt/u8500_initramfs_files/init $(PRODUCT_OUT)/u8500_initramfs_files/init
	cp $(LOCAL_PATH)/prebuilt/codina_initramfs.list $(PRODUCT_OUT)/codina_initramfs.list
	cp $(LOCAL_PATH)/prebuilt/janice_initramfs.list $(PRODUCT_OUT)/janice_initramfs.list
	#cp $(LOCAL_PATH)/prebuilt/charger.cpio.gz $(PRODUCT_OUT)/charger.cpio.gz
	cp $(LOCAL_PATH)/prebuilt/prebuilt-recovery-codina.cpio.gz $(PRODUCT_OUT)/prebuilt-recovery-codina.cpio.gz
	cp $(LOCAL_PATH)/prebuilt/prebuilt-recovery-janice.cpio.gz $(PRODUCT_OUT)/prebuilt-recovery-janice.cpio.gz
	cp $(recovery_uncompressed_ramdisk) $(PRODUCT_OUT)/recovery.cpio
	gzip -9f $(PRODUCT_OUT)/recovery.cpio

TARGET_KERNEL_BINARIES: $(KERNEL_OUT) $(KERNEL_CONFIG) $(KERNEL_HEADERS_INSTALL) $(recovery_uncompressed_ramdisk) $(uncompressed_ramdisk) $(PRODUCT_OUT)/recovery.cpio.gz
	$(MAKE) -C $(KERNEL_SRC) O=$(KERNEL_OUT) ARCH=$(TARGET_ARCH) $(KERNEL_CROSS_COMPILE) $(TARGET_PREBUILT_INT_KERNEL_TYPE)
	$(MAKE) -C $(KERNEL_SRC) O=$(KERNEL_OUT) ARCH=$(TARGET_ARCH) $(KERNEL_CROSS_COMPILE) modules
	$(MAKE) -C $(KERNEL_SRC) O=$(KERNEL_OUT) INSTALL_MOD_PATH=../../$(KERNEL_MODULES_INSTALL) ARCH=$(TARGET_ARCH) $(KERNEL_CROSS_COMPILE) modules_install

$(PRODUCT_OUT)/install/janice/boot.img:
	mkdir -p $(PRODUCT_OUT)/obj/KERNEL_OBJ_janice
	mkdir -p $(PRODUCT_OUT)/install/janice
	$(MAKE) -C $(KERNEL_SRC) O=$(PRODUCT_OUT)/obj/KERNEL_OBJ_janice ARCH=$(TARGET_ARCH) $(KERNEL_CROSS_COMPILE) janice_defconfig
	$(MAKE) -C $(KERNEL_SRC) O=$(PRODUCT_OUT)/obj/KERNEL_OBJ_janice ARCH=$(TARGET_ARCH) $(KERNEL_CROSS_COMPILE) $(TARGET_PREBUILT_INT_KERNEL_TYPE)
	cp $(PRODUCT_OUT)/obj/KERNEL_OBJ_janice/arch/arm/boot/zImage $(PRODUCT_OUT)/install/janice/boot.img

$(INSTALLED_BOOTIMAGE_TARGET): $(INSTALLED_KERNEL_TARGET) $(PRODUCT_OUT)/install/janice/boot.img
	mkdir -p $(PRODUCT_OUT)/install/codina
	cp $< $(PRODUCT_OUT)/install/codina/boot.img
	touch $@

$(INSTALLED_RECOVERYIMAGE_TARGET): $(INSTALLED_KERNEL_TARGET)
	$(ACP) -fp $< $@
