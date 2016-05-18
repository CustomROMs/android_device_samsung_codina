#!/system/bin/sh

BB=/system/xbin/busybox

LUN0_PATH=/sys/class/android_usb/f_mass_storage/lun0/file
LUN1_PATH=/sys/class/android_usb/f_mass_storage/lun1/file

sdcard0_block=$( $BB mount | $BB grep "sdcard0" | $BB head -n 1 | $BB cut -d " " -f1 )
sdcard1_block=$( $BB mount | $BB grep "sdcard1" | $BB head -n 1 | $BB cut -d " " -f1 )

$BB echo $sdcard0_block > $LUN0_PATH
$BB echo $sdcard1_block > $LUN1_PATH
