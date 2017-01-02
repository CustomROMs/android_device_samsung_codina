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
#define QOS_DDR_OPP_BOOST_DUR_PATH "/sys/module/prcmu_qos_power/parameters/qos_ddr_opp_boost_dur_ms"
#define QOS_DDR_OPP_NORMAL "25"
#define QOS_DDR_OPP_BOOST "100"
#define QOS_DDR_OPP_BOOST_DUR_DEF 8000

#define QOS_APE_OPP_PATH "/sys/module/prcmu_qos_power/parameters/qos_ape_opp"
#define QOS_APE_OPP_BOOST_DUR_PATH "/sys/module/prcmu_qos_power/parameters/qos_ape_opp_boost_dur_ms"
#define QOS_APE_OPP_NORMAL "25"
#define QOS_APE_OPP_BOOST "100"
#define QOS_APE_OPP_BOOST_DUR_DEF 8000

#define QOS_ARM_KHZ_PATH "/sys/module/prcmu_qos_power/parameters/qos_arm_khz"
#define QOS_ARM_KHZ_BOOST_DUR_PATH "/sys/module/prcmu_qos_power/parameters/qos_arm_khz_boost_dur_ms"
#define QOS_ARM_KHZ_NORMAL "200000"
#define QOS_ARM_KHZ_MAX "1200000"

#define GPU_FREQ_MIN_PATH 	"/sys/kernel/mali/mali_boost_low"
#define GPU_FREQ_MAX_PATH 	"/sys/kernel/mali/mali_boost_high"
#define GPU_FREQ_BOOST	 	"499200"
#define GPU_FREQ_NORMAL	 	"399360"
#define GPU_FREQ_LOW	 	"256000"

#define CPU0_BOOST_PULSE_PATH 	"/sys/module/prcmu_qos_power/parameters/qos_arm_khz"
#define CPU0_BOOST_PULSE_FREQ 	"800000"
#define CPU0_BOOST_P_DUR_PATH 	"/sys/module/prcmu_qos_power/parameters/qos_arm_khz_boost_dur_ms"
#define CPU0_BOOST_P_DUR_DEF	8000
#define CPU0_GOV_PATH	 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define CPU0_FREQ_MIN_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU0_FREQ_MAX	 	"1200000\n"
#define CPU0_FREQ_LOW	 	"200000\n"

#define DUR_INFINITE "-1"
#define DUR_ZERO "0"

#define PROP_CPU0_GOV "powerhal.cpu0_gov"
#define PROP_CPU0_FREQ_MIN "powerhal.cpu0_freq_min"
#define PROP_CPU0_FREQ_MAX "powerhal.cpu0_freq_max"

#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST "powerhal.set_interactive.arm_khz_boost"
#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_DUR "powerhal.set_interactive.arm_khz_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST "powerhal.set_interactive.ddr_opp_boost"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR "powerhal.set_interactive.ddr_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST "powerhal.set_interactive.ape_opp_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR "powerhal.set_interactive.ape_opp_boost_duration_ms"

#define PROP_VSYNC_ARM_KHZ_BOOST "powerhal.vsync.arm_khz_boost"
#define PROP_VSYNC_DDR_OPP_BOOST "powerhal.vsync.ddr_opp_boost"
#define PROP_VSYNC_APE_OPP_BOOST "powerhal.vsync.ape_opp_boost"

#define PROP_CPUBOOST_ARM_KHZ_BOOST "powerhal.cpuboost.arm_khz_boost"
#define PROP_CPUBOOST_DUR "powerhal.cpuboost.duration_ms"

#define PROP_GPU_FREQ_MIN "powerhal.gpu.min"
#define PROP_GPU_FREQ_MAX "powerhal.gpu.max"

#endif /* CODINA_H */
