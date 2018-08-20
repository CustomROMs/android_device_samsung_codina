/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2016 Jonathan Jason Dennis [Meticulus]
 *									theonejohnnyd@gmail.com
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

/* Sensor handles */
#define HANDLE_ACCELEROMETER	(0)
#define HANDLE_MAGNETIC_FIELD	(1)
#define HANDLE_ORIENTATION		(2)
#define HANDLE_GYROSCOPE		(3)
#define HANDLE_LIGHT			(4)
#define HANDLE_PRESSURE			(5)
#define HANDLE_TEMPERATURE		(6)
#define HANDLE_PROXIMITY		(8)
#define HANDLE_MAX			    (9)

/* Sensor handles */
#define MINDELAY_ACCELEROMETER	(10000)
#define MINDELAY_MAGNETIC_FIELD	(10000)
#define MINDELAY_ORIENTATION	(10000)
#define MINDELAY_GYROSCOPE		(10000)
#define MINDELAY_LIGHT			(0)
#define MINDELAY_PRESSURE		(1000)
#define MINDELAY_TEMPERATURE	(0)
#define MINDELAY_PROXIMITY		(0)

/* Constants */
#define LSM_M_MAX_CAL_COUNT 300
#define RADIANS_TO_DEGREES (180.0/M_PI)
#define DEGREES_TO_RADIANS (M_PI/180.0)
#define MAX_LENGTH 150
#define SIZE_OF_BUF 100

/* Functions */
#define CONVERT_A  (GRAVITY_EARTH * (1.0f/1000.0f))

/* Alps defines (kyle) */
#define ALPSIO	0xAF
#define ALPSIO_SET_MAGACTIVATE   _IOW(ALPSIO, 0, int)
#define ALPSIO_SET_ACCACTIVATE   _IOW(ALPSIO, 1, int)
#define ALPSIO_SET_DELAY         _IOW(ALPSIO, 2, int)

/* Magnetometer defines */
#define YAS530_POWER 0.3f
#define YAS530_RANGE 1600.0f
#define YAS530_RESOLUTION 6.1f

/* Accelerometer defines */
#define BMA222_POWER  0.5f
#define BMA222_RANGE  10240.0f
#define BMA222_RESOLUTION 1.0f

/* proximity defines */
#define GP2A_POWER 0.75f
#define GP2A_RANGE  5.0f
#define GP2A_RESOLUTION  5.0f

/* Orientation defines */
#define ALPS_POWER 9.7f
#define ALPS_RANGE 360.0f
#define ALPS_RESOLUTION 1.0f

/* magnetometer paths*/
char const *const PATH_DATA_MAG =
		"/sys/class/input/input8/data";

char const *const PATH_POWER_MAG =
		"/sys/class/input/input8/enable";

/* accelerometer paths*/
char const *const PATH_DATA_ACC =
		"/sys/class/sensors/accelerometer_sensor/raw_data";
char const *const PATH_POWER_ACC =
		"/sys/class/input/input7/enable";

/* proximity paths*/
char const *const PATH_POWER_PROX =
		"/sys/class/input/input4/enable";
char const *const PATH_DATA_PROX =
		"/sys/class/sensors/proximity_sensor/adc";
char const *const PATH_INTR_PROX =
                  "/dev/input/event4";

/* alps (kyle) paths */

char const *const PATH_IO_ALPS =
		"/dev/accelirq";

char const *const PATH_DATA_LIGHT = "/sys/class/sensors/light_sensor/lightsensor_lux";
char const *const PATH_POWER_LIGHT = "/sys/class/input/input5/enable";

/* Accelerometer sensor path structure */
typedef struct {
    char path_mode[MAX_LENGTH];
    char path_range[MAX_LENGTH];
    char path_rate[MAX_LENGTH];
    char path_data[MAX_LENGTH];
    char gyro_path_mode[MAX_LENGTH];
    char gyro_path_rate[MAX_LENGTH];
    char gyro_path_data[MAX_LENGTH];
    char gyro_path_sensitivity[MAX_LENGTH];
    char magn_range[MAX_LENGTH];
} Sensor_data;

/* To store all Sensors data*/
typedef struct {
   sensors_event_t sensor_data[8];
   int length;
} Sensor_messagequeue;

typedef struct {
    int prox_val;
    char prox_flag;
}Sensor_prox;

/* sensor API integration */

static const struct sensor_t sSensorList[] = {
        { "MPU3050 Gyroscope sensor",
          "Invensense Technology",
          1, HANDLE_GYROSCOPE,
          SENSOR_TYPE_GYROSCOPE, 10240.0f, 1.0f, 0.5f, 10000, 0, 0, 0, 0, 0, 0, { } },
        { "BMA222 3-axis Accelerometer",
          "Bosch Corporation",
          1, HANDLE_ACCELEROMETER,
          SENSOR_TYPE_ACCELEROMETER, 10240.0f, 1.0f, 0.5f, 10000, 0, 0, 0, 0, 0, 0, { } },
        { "YAS530 Magnetic sensor",
          "YAMAHA Corporation",
          1, HANDLE_MAGNETIC_FIELD,
          SENSOR_TYPE_MAGNETIC_FIELD, 0.3f, 1600.0f, 6.1f, 10000, 0, 0, 0, 0, 0, 0, { } },
        { "YAS530 Orientation sensor",
          "YAMAHA Corporation",
          1, HANDLE_ORIENTATION,
          SENSOR_TYPE_ORIENTATION, 360.0f, 1.0f, 9.7f, 10000, 0, 0, 0, 0, 0, 0, { } },
        { "GP2A Light sensor",
          "Sharp",
          1, HANDLE_LIGHT,
          SENSOR_TYPE_LIGHT,  3000.0f, 1.0f, 0.75f, 0, 0, 0, 0, 0, 0, 0, { } },
        { "GP2A Proximity sensor",
          "Sharp",
          1, HANDLE_PROXIMITY,
          SENSOR_TYPE_PROXIMITY, 5.0f, 5.0f, 0.75f, 0, 0, 0, 0, 0, 0, 0, { } },
};

/*
static const struct sensor_t sSensorList[] = {
	{"GP2A Proximity Sensor",
		"Meticulus/GP2A",
		1,
		HANDLE_PROXIMITY,
		SENSOR_TYPE_PROXIMITY,
		GP2A_RANGE,
		GP2A_RESOLUTION,
		GP2A_POWER,
		MINDELAY_PROXIMITY,
		0,
		0,
		"vu.co.meticulus.taos.proximity",
		"",
		0,
		SENSOR_FLAG_WAKE_UP | SENSOR_FLAG_ON_CHANGE_MODE,
		{0, 0},
	},
	{"YAS530 Magnetic Sensor",
		"alps electric co., ltd.",
		1,
		HANDLE_MAGNETIC_FIELD,
		SENSOR_TYPE_MAGNETIC_FIELD,
		YAS530_RANGE,
		YAS530_RESOLUTION,
		YAS530_POWER,
		MINDELAY_MAGNETIC_FIELD,
		0,
		0,
		"vu.co.meticulus.alps.magnetic",
		"",
		0,
		0,
		{0, 0},
	},
	{"BMA222 Accelerometer",
		"Bosch Corporation",
		1,
		HANDLE_ACCELEROMETER,
		SENSOR_TYPE_ACCELEROMETER,
		BMA222_RANGE,
		BMA222_RESOLUTION,
		BMA222_POWER,
		MINDELAY_ACCELEROMETER,
		0,
		0,
		"vu.co.meticulus.bosch.accerometer",
		"",
		0,
		0,
		{0, 0},
	},
	{"ALPS Orientation Sensor",
		"alps electric co., ltd",
		1,
		HANDLE_ORIENTATION,
		SENSOR_TYPE_ORIENTATION,
		ALPS_RANGE,
		ALPS_RESOLUTION,
		ALPS_POWER,
		MINDELAY_ORIENTATION,
		0,
		0,
		"vu.co.meticulus.alps.orientation",
		"",
		0,
		0,
		{0, 0},
	},
};
*/
static int acc_id;
