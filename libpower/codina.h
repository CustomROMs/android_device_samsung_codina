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

/* DDR OPP */
#define QOS_DDR_OPP_PATH "/sys/module/prcmu_qos_power/parameters/qos_ddr_opp"
#define QOS_DDR_OPP_BOOST_DUR_PATH "/sys/module/prcmu_qos_power/parameters/qos_ddr_opp_boost_dur_ms"
#define QOS_DDR_OPP_NORMAL "25"
#define QOS_DDR_OPP_BOOST "100"

/* APE OPP */
#define QOS_APE_OPP_PATH "/sys/module/prcmu_qos_power/parameters/qos_ape_opp"
#define QOS_APE_OPP_BOOST_DUR_PATH "/sys/module/prcmu_qos_power/parameters/qos_ape_opp_boost_dur_ms"
#define QOS_APE_OPP_NORMAL "25"
#define QOS_APE_OPP_BOOST "100"

/* ARM KHZ */
#define QOS_ARM_KHZ_PATH "/sys/module/prcmu_qos_power/parameters/qos_arm_khz"
#define QOS_ARM_KHZ_BOOST_DUR_PATH "/sys/module/prcmu_qos_power/parameters/qos_arm_khz_boost_dur_ms"
#define QOS_ARM_KHZ_NORMAL "200000"
#define QOS_ARM_KHZ_MAX "1200000"

/* GPU Freq */
#define GPU_FREQ_MIN_PATH 	"/sys/kernel/mali/mali_boost_low"
#define GPU_FREQ_MAX_PATH 	"/sys/kernel/mali/mali_boost_high"
#define GPU_FREQ_BOOST	 	"499200"
#define GPU_FREQ_NORMAL	 	"399360"
#define GPU_FREQ_LOW	 	"256000"

/* CPU Freq */
#define CPU0_BOOST_PULSE_PATH 	"/sys/module/prcmu_qos_power/parameters/qos_arm_khz"
#define CPU0_BOOST_PULSE_FREQ 	"800000"
#define CPU0_BOOST_P_DUR_PATH 	"/sys/module/prcmu_qos_power/parameters/qos_arm_khz_boost_dur_ms"
#define CPU0_GOV_PATH	 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define CPU0_FREQ_MIN_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU0_FREQ_MAX_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

/* Durations */
#define DUR_INFINITE "-1"
#define DUR_ZERO "0"

#define PROP_VSYNC_ARM_KHZ_BOOST "powerhal.vsync.arm_khz_boost"
#define PROP_VSYNC_DDR_OPP_BOOST "powerhal.vsync.ddr_opp_boost"
#define PROP_VSYNC_APE_OPP_BOOST "powerhal.vsync.ape_opp_boost"


/* Start profile power_save  */
#define CPU0_GOV_0 "dynamic"
#define CPU0_FREQ_MIN_0 "200000"
#define CPU0_FREQ_MAX_0 "800000"
#define CPU0_BOOST_PULSE_FREQ_0 "200000"
#define GPU_FREQ_MIN_0 GPU_FREQ_LOW
#define GPU_FREQ_MAX_0 GPU_FREQ_NORMAL
#define QOS_DDR_OPP_BOOST_DUR_DEF_0 0
#define QOS_APE_OPP_BOOST_DUR_DEF_0 0
#define CPU0_BOOST_P_DUR_DEF_0	0
#define CPU_SHOULD_BOOST_0 0
#define GPU_SHOULD_BOOST_0 0
#define DDR_SHOULD_BOOST_0 0

/* Properties */
#define PROP_CPU0_GOV_0 "powerhal.power_save.cpu0_gov"
#define PROP_CPU0_FREQ_MIN_0 "powerhal.power_save.cpu0_freq_min"
#define PROP_CPU0_FREQ_MAX_0 "powerhal.power_save.cpu0_freq_max"

#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_0 "powerhal.power_save.set_interactive.arm_khz_boost"
#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_DUR_0 "powerhal.power_save.set_interactive.arm_khz_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_0 "powerhal.power_save.set_interactive.ddr_opp_boost"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR_0 "powerhal.power_save.set_interactive.ddr_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_SHOULD_BOOST_0 "powerhal.power_save.set_interactive.ddr_opp_should_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_0 "powerhal.power_save.set_interactive.ape_opp_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR_0 "powerhal.power_save.set_interactive.ape_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_APE_OPP_SHOULD_BOOST_0 "powerhal.power_save.set_interactive.ape_opp_should_boost"

#define PROP_CPUBOOST_ARM_KHZ_BOOST_0 "powerhal.power_save.cpuboost.arm_khz_boost"
#define PROP_CPUBOOST_ARM_KHZ_SHOULD_BOOST_0 "powerhal.power_save.cpuboost.arm_khz_should_boost"
#define PROP_CPUBOOST_DUR_0 "powerhal.power_save.cpuboost.duration_ms"

#define PROP_GPU_FREQ_MIN_0 "powerhal.power_save.gpu.min"
#define PROP_GPU_FREQ_MAX_0 "powerhal.power_save.gpu.max"

/* End profile power_save  */

/* Start profile balanced */
#define CPU0_GOV_1 "dynamic"
#define CPU0_FREQ_MIN_1 "200000"
#define CPU0_FREQ_MAX_1 "800000"
#define CPU0_BOOST_PULSE_FREQ_1 "400000"
#define GPU_FREQ_MIN_1 GPU_FREQ_NORMAL
#define GPU_FREQ_MAX_1 GPU_FREQ_NORMAL
#define QOS_DDR_OPP_BOOST_DUR_DEF_1 4000
#define QOS_APE_OPP_BOOST_DUR_DEF_1 4000
#define CPU0_BOOST_P_DUR_DEF_1	4000
#define CPU_SHOULD_BOOST_1 1
#define GPU_SHOULD_BOOST_1 1
#define DDR_SHOULD_BOOST_1 1
#define PROP_CPU0_GOV_1 "powerhal.balanced.cpu0_gov"
#define PROP_CPU0_FREQ_MIN_1 "powerhal.balanced.cpu0_freq_min"
#define PROP_CPU0_FREQ_MAX_1 "powerhal.balanced.cpu0_freq_max"

#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_1 "powerhal.balanced.set_interactive.arm_khz_boost"
#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_DUR_1 "powerhal.balanced.set_interactive.arm_khz_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_1 "powerhal.balanced.set_interactive.ddr_opp_boost"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR_1 "powerhal.balanced.set_interactive.ddr_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_SHOULD_BOOST_1 "powerhal.balanced.set_interactive.ddr_opp_should_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_1 "powerhal.balanced.set_interactive.ape_opp_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR_1 "powerhal.balanced.set_interactive.ape_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_APE_OPP_SHOULD_BOOST_1 "powerhal.balanced.set_interactive.ape_opp_should_boost"

#define PROP_CPUBOOST_ARM_KHZ_BOOST_1 "powerhal.balanced.cpuboost.arm_khz_boost"
#define PROP_CPUBOOST_ARM_KHZ_SHOULD_BOOST_1 "powerhal.balanced.cpuboost.arm_khz_should_boost"
#define PROP_CPUBOOST_DUR_1 "powerhal.balanced.cpuboost.duration_ms"

#define PROP_GPU_FREQ_MIN_1 "powerhal.balanced.gpu.min"
#define PROP_GPU_FREQ_MAX_1 "powerhal.balanced.gpu.max"

/* End profile balanced  */

/* Start profile performance  */
#define CPU0_GOV_2 "dynamic"
#define CPU0_FREQ_MIN_2 "200000"
#define CPU0_FREQ_MAX_2 "1000000"
#define CPU0_BOOST_PULSE_FREQ_2 "800000"
#define GPU_FREQ_MIN_2 GPU_FREQ_NORMAL
#define GPU_FREQ_MAX_2 GPU_FREQ_BOOST
#define QOS_DDR_OPP_BOOST_DUR_DEF_2 8000
#define QOS_APE_OPP_BOOST_DUR_DEF_2 8000
#define CPU0_BOOST_P_DUR_DEF_2	8000
#define CPU_SHOULD_BOOST_2 1
#define GPU_SHOULD_BOOST_2 1
#define DDR_SHOULD_BOOST_2 1

#define PROP_CPU0_GOV_2 "powerhal.performance.cpu0_gov"
#define PROP_CPU0_FREQ_MIN_2 "powerhal.performance.cpu0_freq_min"
#define PROP_CPU0_FREQ_MAX_2 "powerhal.performance.cpu0_freq_max"

#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_2 "powerhal.performance.set_interactive.arm_khz_boost"
#define PROP_SET_INTERACTIVE_ARM_KHZ_BOOST_DUR_2 "powerhal.performance.set_interactive.arm_khz_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_2 "powerhal.performance.set_interactive.ddr_opp_boost"
#define PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR_2 "powerhal.performance.set_interactive.ddr_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_DDR_OPP_SHOULD_BOOST_2 "powerhal.performance.set_interactive.ddr_opp_should_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_2 "powerhal.performance.set_interactive.ape_opp_boost"
#define PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR_2 "powerhal.performance.set_interactive.ape_opp_boost_duration_ms"
#define PROP_SET_INTERACTIVE_APE_OPP_SHOULD_BOOST_2 "powerhal.performance.set_interactive.ape_opp_should_boost"

#define PROP_CPUBOOST_ARM_KHZ_BOOST_2 "powerhal.performance.cpuboost.arm_khz_boost"
#define PROP_CPUBOOST_ARM_KHZ_SHOULD_BOOST_2 "powerhal.performance.cpuboost.arm_khz_should_boost"
#define PROP_CPUBOOST_DUR_2 "powerhal.performance.cpuboost.duration_ms"

#define PROP_GPU_FREQ_MIN_2 "powerhal.performance.gpu.min"
#define PROP_GPU_FREQ_MAX_2 "powerhal.performance.gpu.max"

/* End profile performance  */

struct power_profile {
    const char * cpu0_gov;
    const char * prop_cpu0_gov;
    const char * cpu0_freq_max;
    const char * prop_cpu0_freq_max;
    const char * cpu0_freq_min;
    const char * prop_cpu0_freq_min;
    const int    cpu0_boost_pulse_freq;
    const char * prop_cpu0_boost_pulse_freq;
    const int    cpu0_boost_p_dur_def;
    const char * prop_cpu0_boost_p_dur_def;
    const int    cpu0_should_boost;
    const char * prop_cpu0_should_boost;
    const char * gpu_freq_max;
    const char * prop_gpu_freq_max;
    const char * gpu_freq_min;
    const char * prop_gpu_freq_min;
    const int    gpu_should_boost;
    const char * prop_gpu_should_boost;
    const int    gpu_boost_dur_def;
    const char * prop_gpu_boost_dur_def;
    const int	 ddr_should_boost;
    const char * prop_ddr_should_boost;
    const int	 ddr_boost_dur_def;
    const char * prop_ddr_boost_dur_def;
};

struct power_profile power_save = {
    .cpu0_gov = CPU0_GOV_0,
    .prop_cpu0_gov = PROP_CPU0_GOV_0,
    .cpu0_freq_max = CPU0_FREQ_MAX_0,
    .prop_cpu0_freq_max = PROP_CPU0_FREQ_MAX_0,
    .cpu0_freq_min = CPU0_FREQ_MIN_0,
    .prop_cpu0_freq_min = PROP_CPU0_FREQ_MIN_0,
    .cpu0_boost_pulse_freq = CPU0_BOOST_PULSE_FREQ_0,
    .prop_cpu0_boost_pulse_freq = PROP_CPUBOOST_ARM_KHZ_BOOST_0,
    .cpu0_boost_p_dur_def = CPU0_BOOST_P_DUR_DEF_0,
    .prop_cpu0_boost_p_dur_def = PROP_CPUBOOST_DUR_0,
    .cpu0_should_boost = CPU_SHOULD_BOOST_0,
    .prop_cpu0_should_boost = PROP_CPUBOOST_ARM_KHZ_SHOULD_BOOST_0,
    .gpu_freq_max = GPU_FREQ_MAX_0,
    .prop_gpu_freq_max = PROP_GPU_FREQ_MAX_0,
    .gpu_freq_min = GPU_FREQ_MIN_0,
    .prop_gpu_freq_min = PROP_GPU_FREQ_MIN_0,
    .gpu_should_boost = GPU_SHOULD_BOOST_0,
    .prop_gpu_should_boost = PROP_SET_INTERACTIVE_APE_OPP_BOOST_0,
    .gpu_boost_dur_def = QOS_APE_OPP_BOOST_DUR_DEF_0,
    .prop_gpu_boost_dur_def = PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR_0,
    .ddr_should_boost = DDR_SHOULD_BOOST_0,
    .prop_ddr_should_boost = PROP_SET_INTERACTIVE_DDR_OPP_BOOST_0,
    .ddr_boost_dur_def = QOS_DDR_OPP_BOOST_DUR_DEF_0,
    .prop_ddr_boost_dur_def = PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR_0,
};

struct power_profile balanced = {
    .cpu0_gov = CPU0_GOV_1,
    .prop_cpu0_gov = PROP_CPU0_GOV_1,
    .cpu0_freq_max = CPU0_FREQ_MAX_1,
    .prop_cpu0_freq_max = PROP_CPU0_FREQ_MAX_1,
    .cpu0_freq_min = CPU0_FREQ_MIN_1,
    .prop_cpu0_freq_min = PROP_CPU0_FREQ_MIN_1,
    .cpu0_boost_pulse_freq = CPU0_BOOST_PULSE_FREQ_1,
    .prop_cpu0_boost_pulse_freq = PROP_CPUBOOST_ARM_KHZ_BOOST_1,
    .cpu0_boost_p_dur_def = CPU0_BOOST_P_DUR_DEF_1,
    .prop_cpu0_boost_p_dur_def = PROP_CPUBOOST_DUR_1,
    .cpu0_should_boost = CPU_SHOULD_BOOST_1,
    .prop_cpu0_should_boost = PROP_CPUBOOST_ARM_KHZ_SHOULD_BOOST_1,
    .gpu_freq_max = GPU_FREQ_MAX_1,
    .prop_gpu_freq_max = PROP_GPU_FREQ_MAX_1,
    .gpu_freq_min = GPU_FREQ_MIN_1,
    .prop_gpu_freq_min = PROP_GPU_FREQ_MIN_1,
    .gpu_should_boost = GPU_SHOULD_BOOST_1,
    .prop_gpu_should_boost = PROP_SET_INTERACTIVE_APE_OPP_BOOST_1,
    .gpu_boost_dur_def = QOS_APE_OPP_BOOST_DUR_DEF_1,
    .prop_gpu_boost_dur_def = PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR_1,
    .ddr_should_boost = DDR_SHOULD_BOOST_1,
    .prop_ddr_should_boost = PROP_SET_INTERACTIVE_DDR_OPP_BOOST_1,
    .ddr_boost_dur_def = QOS_DDR_OPP_BOOST_DUR_DEF_1,
    .prop_ddr_boost_dur_def = PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR_1,
};

struct power_profile performance = {
    .cpu0_gov = CPU0_GOV_2,
    .prop_cpu0_gov = PROP_CPU0_GOV_2,
    .cpu0_freq_max = CPU0_FREQ_MAX_2,
    .prop_cpu0_freq_max = PROP_CPU0_FREQ_MAX_2,
    .cpu0_freq_min = CPU0_FREQ_MIN_2,
    .prop_cpu0_freq_min = PROP_CPU0_FREQ_MIN_2,
    .cpu0_boost_pulse_freq = CPU0_BOOST_PULSE_FREQ_2,
    .prop_cpu0_boost_pulse_freq = PROP_CPUBOOST_ARM_KHZ_BOOST_2,
    .cpu0_boost_p_dur_def = CPU0_BOOST_P_DUR_DEF_2,
    .prop_cpu0_boost_p_dur_def = PROP_CPUBOOST_DUR_2,
    .cpu0_should_boost = CPU_SHOULD_BOOST_2,
    .prop_cpu0_should_boost = PROP_CPUBOOST_ARM_KHZ_SHOULD_BOOST_2,
    .gpu_freq_max = GPU_FREQ_MAX_2,
    .prop_gpu_freq_max = PROP_GPU_FREQ_MAX_2,
    .gpu_freq_min = GPU_FREQ_MIN_2,
    .prop_gpu_freq_min = PROP_GPU_FREQ_MIN_2,
    .gpu_should_boost = GPU_SHOULD_BOOST_2,
    .prop_gpu_should_boost = PROP_SET_INTERACTIVE_APE_OPP_BOOST_2,
    .gpu_boost_dur_def = QOS_APE_OPP_BOOST_DUR_DEF_2,
    .prop_gpu_boost_dur_def = PROP_SET_INTERACTIVE_APE_OPP_BOOST_DUR_2,
    .ddr_should_boost = DDR_SHOULD_BOOST_2,
    .prop_ddr_should_boost = PROP_SET_INTERACTIVE_DDR_OPP_BOOST_2,
    .ddr_boost_dur_def = QOS_DDR_OPP_BOOST_DUR_DEF_2,
    .prop_ddr_boost_dur_def = PROP_SET_INTERACTIVE_DDR_OPP_BOOST_DUR_2,
};

#endif /* CODINA_H */
