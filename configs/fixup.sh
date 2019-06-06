#!/sbin/sh

if grep -q janice /default.prop || grep -iq i9070 /default.prop ; then
cp -r /tmp/janice/* /system/
#sed -i "s,8160,9070,g" /tmp/build.prop
#sed -i "s,codina,janice,g" /tmp/build.prop
#cp /tmp/build.prop /system/
dd if=/tmp/install/janice/boot.img of=/dev/block/mmcblk0p15 bs=1M
sync
elif grep -q codina /default.prop || grep -iq i8160 /default.prop ; then
cp -r /tmp/codina/* /system/
dd if=/tmp/install/codina/boot.img of=/dev/block/mmcblk0p15 bs=1M
sync
fi
