/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <string.h>
#include <errno.h>
#include <limits.h>

#include <utils/Log.h>
#include <cutils/properties.h>

#include "PowerHAL.h"

using namespace android;

int main(int argc, char *argv[])
{
	PowerHAL mPowerHAL;
	const char *profile, *err_str;
	int status = -EINVAL;
	int profile_num;
	void *handle = NULL;

	if (argc <= 1) {
		printf("Please specify a profile (0 - power_save, 1 - balanced, 2 - performance)!\n");
		return status;
	}

	profile = argv[1];

	status = sscanf(profile, "%d", &profile_num) == 1 ? 0 : -EINVAL;
	if (status || profile_num < 0 || profile_num > 2) {
		printf("invalid input: %s", profile);
		return status;
	}

	mPowerHAL.set_profile(profile_num);

	return status;
}
