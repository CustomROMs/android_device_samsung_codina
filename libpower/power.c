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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_TAG "Met-Dev Hisi PowerHAL"
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

static void write_string(char * path, char * value) {
    int fd = open(path, O_WRONLY);
	if(!fd) { ALOGE("Unable to open to %s", path); return;}

	ssize_t bytes_written = write(fd, value, strlen(value));

	if (bytes_written < 1 || bytes_written < strlen(value)) {
		ALOGE("Unable to write to %s : %d",path, bytes_written);
	}

    close(fd);

}

static void power_init(struct power_module *module)
{
#ifdef DEBUG
		ALOGE("init");
#endif

    //write_string(CPU0_GOV_PATH, "dynamic\n");
}

static void power_set_interactive(struct power_module *module, int on) {
#ifdef DEBUG
		ALOGE("set_interactive %d", on);
#endif

	if (on) {
	    write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_BOOST);
	    write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_BOOST);
	} else {
	    write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_NORMAL);
	    write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_NORMAL);
	}
}

static void power_hint_cpu_boost(int dur) {
//TODO
#if 0
    char sdur[255];

    if(!dur)
	dur = CPU0_BOOST_P_DUR_DEF;

    sprintf(sdur, "%d\n", dur);
    write_string(CPU0_BOOST_P_DUR_PATH,sdur);
    write_string(CPU0_BOOST_PULSE_PATH,"1\n");
#endif
}

static void power_hint_interactive(int on) {
	power_hint_cpu_boost(on);
//TODO
#if 0
	write_string(GPU_ANIM_BOOST_PATH,"1\n");
#endif
}

static void power_hint_vsync(int on) {
	if (on) {
	    write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_BOOST);
	    write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_BOOST);
	} else {
	    write_string(QOS_DDR_OPP_PATH, QOS_DDR_OPP_NORMAL);
	    write_string(QOS_APE_OPP_PATH, QOS_APE_OPP_NORMAL);
	}
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
                DEBUG_LOG("POWER_HINT_VSYNC %d", var);
                power_hint_vsync(var);
                break;
        case POWER_HINT_INTERACTION:
                if(data != NULL)
                    var = *(int *) data;
                DEBUG_LOG("POWER_HINT_INTERACTION %d", var);
                power_hint_interactive(var);
                break;
	case POWER_HINT_LOW_POWER:
		DEBUG_LOG("POWER_HINT_LOW_POWER %d", var);
		break;
	case POWER_HINT_CPU_BOOST:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_CPU_BOOST %d", var);
		power_hint_cpu_boost(var);
		break;
	case POWER_HINT_LAUNCH_BOOST:
		packageName = ((launch_boost_info_t *)data)->packageName;
		pid = ((launch_boost_info_t *)data)->pid;

		/* Meticulus: not quite sure what to do with this info?
		 * Set thread prio on the app???
		 */
		DEBUG_LOG("POWER_HINT_LAUNCH_BOOST app=%s pid=%d", packageName,pid);
		power_hint_interactive(0);
		break;
	case POWER_HINT_AUDIO:
		DEBUG_LOG("POWER_HINT_AUDIO %d", var);
		break;
	case POWER_HINT_SET_PROFILE:
		DEBUG_LOG("POWER_HINT_PROFILE %d", var);
		break;
    default:
		ALOGE("Unknown power hint %d", var);
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
        .name = "Met-Dev Hisi Power HAL",
        .author = "Meticulus Development",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
};
