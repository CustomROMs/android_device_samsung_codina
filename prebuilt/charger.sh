#!/sbin/sh

if grep -q lpm_boot=1 /proc/cmdline ; then
    echo "starting charger" >> /dev/kmsg
    echo 1 > /sys/class/power_supply/battery/batt_lp_charging
    # Set CPUs governor in LPM
    echo conservative > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor conservative
    echo conservative > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor conservative
    start charger
fi
