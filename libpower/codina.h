/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2016 Jonathan Jason Dennis [Meticulus]
 *					theonejohnnyd@gmail.com
 * Copyright (C) 2017 Shilin Victor Sergeevich [ChronoMonochrome]
 *					chrono.monochrome@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CODINA_H
#define CODINA_H

#define QOS_DDR_OPP_PATH "/sys/module/prcmu_qos_power/parameters/qos_ddr_opp"
#define QOS_DDR_OPP_NORMAL "25"
#define QOS_DDR_OPP_BOOST "100"

#define QOS_APE_OPP_PATH "/sys/module/prcmu_qos_power/parameters/qos_ape_opp"
#define QOS_APE_OPP_NORMAL "25"
#define QOS_APE_OPP_BOOST "100"

#define QOS_ARM_KHZ_PATH "/sys/module/prcmu_qos_power/parameters/qos_arm_khz"
#define QOS_ARM_KHZ_NORMAL "200000"
#define QOS_ARM_KHZ_MAX "1200000"

#define GPU_FREQ_MIN_PATH 	"/sys/class/devfreq/gpufreq/min_freq"
#define GPU_FREQ_BOOST	 	"idx=21" // 700800 kHz
#define GPU_FREQ_NORMAL	 	"idx=5" // 400000 kHz
#define GPU_FREQ_LOW	 	"idx=5"

#define CPU0_BOOST_PULSE_PATH 	"/sys/module/prcmu_qos_power/parameters/qos_arm_khz"
#define CPU0_BOOST_PULSE_FREQ 	"800000"
#define CPU0_BOOST_P_DUR_PATH 	"/sys/module/prcmu_qos_power/parameters/qos_arm_khz_boost_dur_ms"
#define CPU0_BOOST_P_DUR_DEF	8000
#define CPU0_GOV_PATH	 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define CPU0_FREQ_MIN_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU0_FREQ_MAX	 	"1200000\n"
#define CPU0_FREQ_LOW	 	"200000\n"

#endif /* CODINA_H */
