#!/sbin/sh

if grep -q janice /default.prop || grep -iq i9070 /default.prop ; then
flash_image /dev/block/mmcblk0p15 /tmp/install/janice/boot.img
cp -r /tmp/janice/* /system/
elif grep -q codina /default.prop || grep -iq i8160 /default.prop ; then
flash_image /dev/block/mmcblk0p15 /tmp/install/codina/boot.img
cp -r /tmp/codina/* /system/
fi
