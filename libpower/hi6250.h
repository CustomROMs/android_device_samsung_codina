/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2016 Jonathan Jason Dennis [Meticulus]
 *								theonejohnnyd@gmail.com
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

#define DDR_FREQ_MIN_PATH 	"/sys/class/devfreq/ddrfreq/min_freq"
#define DDR_FREQ_POLL	 	"/sys/class/devfreq/ddrfreq/polling_interval"
#define DDR_FREQ_BOOST	 	"800000000\n"
#define DDR_FREQ_NORMAL	 	"533000000\n"
#define DDR_FREQ_LOW	 	"120000000\n"

#define GPU_FREQ_MIN_PATH 	"/sys/class/devfreq/gpufreq/min_freq"
#define GPU_ANIM_BOOST_PATH	"/sys/class/devfreq/gpufreq/mali_ondemand/animation_boost"
#define GPU_FREQ_BOOST	 	"800000000\n"
#define GPU_FREQ_NORMAL	 	"120000000\n"
#define GPU_FREQ_LOW	 	"120000000\n"

#define CPU0_BOOST_PULSE_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/interactive/boostpulse"
#define CPU0_BOOST_P_DUR_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/interactive/boostpulse_duration"
#define CPU0_BOOST_P_DUR_DEF	8000
#define CPU0_GOV_PATH	 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define CPU0_FREQ_MIN_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU0_FREQ_MAX	 	"1709000\n"
#define CPU0_FREQ_LOW	 	"480000\n"

#define CPU4_BOOST_PULSE_PATH 	"/sys/devices/system/cpu/cpu4/cpufreq/interactive/boostpulse"
#define CPU4_GOV_PATH	 	"/sys/devices/system/cpu/cpu4/cpufreq/scaling_governor"
#define CPU4_FREQ_MIN_PATH	"/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq"
#define CPU4_FREQ_MAX	 	"2016000\n"
#define CPU4_FREQ_LOW	 	"1402000\n"
