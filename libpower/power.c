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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_TAG "PowerHAL"
#include <cutils/compiler.h>
#include <cutils/properties.h>
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "codina.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_LOG(x...) ALOGD(x)
#else
#define DEBUG_LOG(x...) do {} while(0)
#endif

static int low_power = 0;
static int dt2w = 0;
static struct power_profile *profile = &performance;
static struct power_module *global_module;
static int current_profile = 2;
static int prev_profile = 2;
static int is_first_init = 1;

static void write_string(char *path, char *value) {
    int fd = open(path, O_WRONLY);
	if(!fd) { ALOGE("Unable to open to %s", path); return;}

	ssize_t bytes_written = write(fd, value, strlen(value));

	if (bytes_written < 1 || bytes_written < strlen(value)) {
		ALOGE("Unable to write to %s : %d",path, bytes_written);
	}

    close(fd);

}

static void write_string_from_prop(char *path, char *prop, char *def_val) {
        char value[PROPERTY_VALUE_MAX];
	DEBUG_LOG("%s: %s, %s", __func__, prop, def_val);
        property_get(prop, value, def_val);
        write_string(path, value);
}

#if 0
static int property_set_(char *key, char *val) {
	ALOGE("%s: %s, %s", __func__, key, val);
	return property_set(key, val);
}
#endif

static void power_init(struct power_module *module)
{
    DEBUG_LOG("init");

    global_module = module;
#if 0
    if (CC_UNLIKELY(is_first_init)) {
        if (property_set_((*profile).prop_cpu0_gov, (*profile).cpu0_gov))
		goto err;
        if (property_set_((*profile).prop_cpu0_freq_max, (*profile).cpu0_freq_max))
		goto err;
        if (property_set_((*profile).prop_cpu0_freq_min, (*profile).cpu0_freq_min))
		goto err;
        if (property_set_((*profile).prop_gpu_freq_max, (*profile).gpu_freq_max))
		goto err;
        if (property_set_((*profile).prop_gpu_freq_min, (*profile).gpu_freq_min))
		goto err;

        is_first_init = 0;
    }
#endif
    /*
     * Chrono: properies can't be accessed at boot?
     * Accessing them too early will result in crash,
     * so skip first power_init call.
     */

    if (CC_UNLIKELY(is_first_init)) {
        is_first_init = 0;
        return;
    }
    write_string_from_prop(CPU0_GOV_PATH, (*profile).prop_cpu0_gov,(*profile).cpu0_gov);
    write_string_from_prop(CPU0_FREQ_MAX_PATH, (*profile).prop_cpu0_freq_max, (*profile).cpu0_freq_max);
    write_string_from_prop(CPU0_FREQ_MIN_PATH, (*profile).prop_cpu0_freq_min, (*profile).cpu0_freq_min);
    write_string_from_prop(GPU_FREQ_MIN_PATH, (*profile).prop_gpu_freq_max, (*profile).gpu_freq_max);
    write_string_from_prop(GPU_FREQ_MAX_PATH, (*profile).prop_gpu_freq_min, (*profile).gpu_freq_min);

    return;

err:
    ALOGE("some of properties can't be set to its defaults, abort!");
}

static void power_set_interactive(struct power_module *module, int on) {
        DEBUG_LOG("set_interactive %d", on);

	if (on & !low_power) {
    	    write_string(QOS_DDR_OPP_BOOST_DUR_PATH, DUR_INFINITE);
            write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_BOOST);

    	    write_string(QOS_APE_OPP_BOOST_DUR_PATH, DUR_INFINITE);
            write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_BOOST);

	} else {
    	    write_string(QOS_DDR_OPP_BOOST_DUR_PATH, DUR_INFINITE);
            write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_NORMAL);

    	    write_string(QOS_APE_OPP_BOOST_DUR_PATH, DUR_INFINITE);
            write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_NORMAL);
	}
}

static void power_hint_cpu_boost(int dur) {
    char sdur[255];
    if (!dur)
        dur = (*profile).cpu0_boost_pulse_freq;

    sprintf(sdur, "%d", dur);
    write_string_from_prop(CPU0_BOOST_P_DUR_PATH, (*profile).prop_cpu0_boost_p_dur_def, sdur);
    write_string_from_prop(CPU0_BOOST_PULSE_PATH, (*profile).prop_cpu0_boost_pulse_freq,
							(*profile).cpu0_boost_pulse_freq);
}

static void power_hint_interactive(int on) {
   char sdur[255];
   int dur = on;
   int cpu_should_boost = property_get_bool((*profile).prop_cpu0_should_boost,
                                        (*profile).cpu0_should_boost);

   int gpu_should_boost = property_get_bool((*profile).prop_gpu_should_boost,
					(*profile).gpu_should_boost);

   int ddr_should_boost = property_get_bool((*profile).prop_ddr_should_boost,
					(*profile).ddr_should_boost);


   if (cpu_should_boost) {
           DEBUG_LOG("%s: boosting CPU", __func__);
	   if (!on)
	       dur = (*profile).cpu0_boost_p_dur_def;

	   power_hint_cpu_boost(dur);
   }

   if (ddr_should_boost) {
           DEBUG_LOG("%s: boosting DDR", __func__);
	   if (!on)
	       dur = (*profile).ddr_boost_dur_def;

	   sprintf(sdur, "%d", dur);

	   write_string_from_prop(QOS_DDR_OPP_BOOST_DUR_PATH,
			  (*profile).prop_ddr_boost_dur_def, sdur);

	   write_string/*_from_prop*/(QOS_DDR_OPP_PATH,
				  /*PROP_SET_INTERACTIVE_DDR_OPP_BOOST, */
						   QOS_DDR_OPP_BOOST);
   }

   if (gpu_should_boost) {
           DEBUG_LOG("%s: boosting GPU", __func__);
	   if (!on)
	       dur = (*profile).gpu_boost_dur_def;

	   sprintf(sdur, "%d", dur);
	   write_string_from_prop(QOS_APE_OPP_BOOST_DUR_PATH,
				  (*profile).prop_gpu_boost_dur_def, sdur);

	   write_string/*_from_prop*/(QOS_APE_OPP_PATH,
				  /*PROP_SET_INTERACTIVE_APE_OPP_BOOST,*/
						   QOS_APE_OPP_BOOST);
   }
}

static void power_hint_vsync(int on) {
//FIXME: setting APE/DDR_OPP to 100 here makes no visual changes
#if 0
	if (on) {
	    write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_BOOST);
	    write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_BOOST);
	} else {
	    write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_NORMAL);
	    write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_NORMAL);
	}
#endif
}

static void power_hint_set_profile(struct power_module *module, int p) {

    switch(p) {
	case 0:
	    profile = &power_save;
	    power_init(module);
	    ALOGI("Set power save profile.");
	    break;
	case 1:
	    profile = &balanced;
	    power_init(module);
	    ALOGI("Set balanced profile.");
	    break;
	case 2:
	    profile = &performance;
	    power_init(module);
	    ALOGI("Set performance profile.");
	    break;
        default:
	    ALOGE("Unknown power profile %d", p);
	    break;
    }

    prev_profile = current_profile;
    current_profile = p;
}

static void power_hint_low_power(int on) {
    if (on && (!low_power))
       power_hint_set_profile(global_module, 0);
    else if ((!on) && low_power)
       power_hint_set_profile(global_module, prev_profile);
    else
       ALOGE("%s: setting the same state %d will not change profile", __func__, on);

    low_power = on;
}

static void power_hint(struct power_module *module, power_hint_t hint,
                        void *data) {
     int var = 0;
     char * packageName;
     int pid = 0;
     switch (hint) {
        case POWER_HINT_VSYNC:
                if(data != NULL)
                    var = *(int *) data;
                //DEBUG_LOG("POWER_HINT_VSYNC %d", var);
		if (!low_power)
	                power_hint_vsync(var);
                break;
        case POWER_HINT_INTERACTION:
                if(data != NULL)
                    var = *(int *) data;
                DEBUG_LOG("POWER_HINT_INTERACTION %d", var);
		if (!low_power)
	                power_hint_interactive(var);
                break;
	case POWER_HINT_LOW_POWER:
		DEBUG_LOG("POWER_HINT_LOW_POWER %d", var);
		break;
	case POWER_HINT_CPU_BOOST:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_CPU_BOOST %d", var);
		if (!low_power)
			power_hint_cpu_boost(var);
		break;
	case POWER_HINT_LAUNCH_BOOST:
		packageName = ((launch_boost_info_t *)data)->packageName;
		pid = ((launch_boost_info_t *)data)->pid;

		/* Meticulus: not quite sure what to do with this info?
		 * Set thread prio on the app???
		 */
		DEBUG_LOG("POWER_HINT_LAUNCH_BOOST app=%s pid=%d", packageName,pid);
		if(!low_power)
		    power_hint_interactive(0);
		break;
	case POWER_HINT_AUDIO:
		DEBUG_LOG("POWER_HINT_AUDIO %d", var);
		ALOGI("Meticulus: POWER_HINT_AUDIO is used! Implement!");
		break;
	case POWER_HINT_SET_PROFILE:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_PROFILE %d", var);
		ALOGI("Meticulus: POWER_SET_PROFILE %d", var);
		power_hint_set_profile(module, var);
		break;
        default:
		ALOGE("Unknown power hint %d", hint);
        	break;
    }
}

#if 0
static void set_dt2w(int on) {
    dt2w = on;
    if(on)
	write_string(WAKE_CONF_PATH,"1\n");
    else
	write_string(WAKE_CONF_PATH,"0\n");
}
#endif

static int get_feature(struct power_module *module, feature_t feature) {
    int retval = 0;
    switch(feature) {
	case POWER_FEATURE_SUPPORTED_PROFILES:
	    retval = 3;
	    break;
	case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
#if 0
	    retval = 1;
#endif
	    break;
        default:
	    ALOGE("Unknown feature %d", feature);
            break;
    }
    return retval;
}

static void set_feature(struct power_module *module, feature_t feature, int state) {
    switch(feature) {
	case POWER_FEATURE_SUPPORTED_PROFILES:
	    ALOGI("POWER_FEATURE_SUPPORTED_PROFILES: %d",state);
	    break;

	case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
#if 0
	    set_dt2w(state);
#endif
	    break;
        default:
	    ALOGE("Unknown feature %d", feature);
            break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_3,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "Power HAL",
        .author = "AOSP",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
    .setFeature = set_feature,
    .getFeature = get_feature,
};
