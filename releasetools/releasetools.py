def FullOTA_InstallEnd(info):
#	info.script.AppendExtra('symlink("/system/lib/modules/tun.ko", "/system/lib/modules/autoload/tun.ko");')
	info.script.AppendExtra('symlink("/system/lib/modules/cpufreq_zenx.ko", "/system/lib/modules/autoload/cpufreq_zenx.ko");')
	info.script.AppendExtra('symlink("/system/lib/modules/cpufreq_ondemandplus.ko", "/system/lib/modules/autoload/cpufreq_ondemandplus.ko");')
	info.script.AppendExtra('symlink("/system/lib/modules/logger.ko", "/system/lib/modules/autoload/logger.ko");')
# Symlink some dependencies of libste_cscall.so
	info.script.AppendExtra('symlink("/system/lib/ste_omxcomponents/libste_dec_amr.so", "/system/lib/libste_dec_amr.so");')
	info.script.AppendExtra('symlink("/system/lib/ste_omxcomponents/libste_enc_amr.so", "/system/lib/libste_enc_amr.so");')
	info.script.AppendExtra('symlink("/system/lib/ste_omxcomponents/libste_dec_amrwb.so", "/system/lib/libste_dec_amrwb.so");')
	info.script.AppendExtra('symlink("/system/lib/ste_omxcomponents/libste_enc_amrwb.so", "/system/lib/libste_enc_amrwb.so");')
#	info.script.AppendExtra('run_program("/sbin/make_ext4fs", "/dev/block/mmcblk0p9");')

def FullOTA_InstallNew(info):
	info.script.AppendExtra('run_program("/sbin/busybox", "mkdir", "/ramdisk");')
	info.script.AppendExtra('run_program("/tmp/install/bin/main.sh", "wipe_log");')
	info.script.AppendExtra('run_program("/tmp/install/bin/main.sh", "check_ramdisk_partition");')
	info.script.AppendExtra('run_program("/tmp/install/bin/main.sh", "check_recovery");')
