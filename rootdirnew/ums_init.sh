#!/system/bin/sh

BB=/system/xbin/busybox

LUN0_PATH=/sys/class/android_usb/f_mass_storage/lun0/file
LUN1_PATH=/sys/class/android_usb/f_mass_storage/lun1/file

sdcard0_block=$( $BB mount | $BB grep "sdcard0" | $BB head -n 1 | $BB cut -d " " -f1 )
sdcard1_block=$( $BB mount | $BB grep "sdcard1" | $BB head -n 1 | $BB cut -d " " -f1 )

sd0_optdevice=$( getprop sys.config.sd0_optdevice )
sd1_optdevice=$( getprop sys.config.sd1_optdevice )

if [ "$sd0_optdevice" == "" ] ; then
     $BB echo $sdcard0_block > $LUN0_PATH
else
      if [ "$sd0_optdevice" != "/dev/null" ] ; then
         $BB echo $sd0_optdevice > $LUN0_PATH
      fi
fi

if [ "$sd1_optdevice" == "" ] ; then
    $BB echo $sdcard1_block > $LUN1_PATH
else
      if [ "$sd1_optdevice" != "/dev/null" ] ; then
         $BB echo $sd1_optdevice > $LUN0_PATH
      fi
fi