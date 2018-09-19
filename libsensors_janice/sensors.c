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

 /* Meticulus Codinalte Notes:
   * Alps seems to be a manager over the Accelerometer/Orienatation sensor and the Magnetic
   * sensor. Currently this lib is using sys fs paths to get readings on those sensors but alps registers
   * these as input devices too. So if it is discovered that this is advantagous switching to input could be done.
   * Oddly, our Alps input driver is alps-input_kyle although "Kyle" is, i believe, a different Novathor device.
   * Our sensor drivers are:
   * Magnetic - 					drivers/sensor/alps/hscdtd008a_i2c.c
   * Accelerometer/Orientation - 	drivers/sensor/alps/bma254.c
   * Proximity  - 					drivers/sensor/proximity/tmd2672.c
   */

#define DEBUG 0

#define LOG_TAG "Sensors"
#include <hardware/sensors.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>

#include <linux/input.h>
#include <sys/time.h>
#include <sys/mman.h>

#include <cutils/atomic.h>
#include <cutils/log.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <linux/ioctl.h>

#include "sensors.h"

static unsigned int count_mag;
static unsigned int count_acc;
static unsigned int count_prox;
static unsigned int count_orien;

unsigned int delay_mag = MINDELAY_MAGNETIC_FIELD;
unsigned int delay_acc = MINDELAY_ACCELEROMETER;
unsigned int delay_prox = MINDELAY_PROXIMITY;
unsigned int delay_orient = MINDELAY_ORIENTATION;

static pthread_cond_t data_available_cv;

static Sensor_messagequeue  stsensor_msgqueue;

static pthread_mutex_t sensordata_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_proxval = PTHREAD_MUTEX_INITIALIZER;
int continue_next;
int events = 0;
static int count_open_sensors = 0;
static int count_orient = 0;
static int count_light = 0;
static int count_delay_sensors = 0;
static int orient_thread_exit = 0;

Sensor_prox  stprox_val;


void *proximity_getdata();
void *acc_getdata();
void *light_getdata();
void *mag_getdata();
void *orient_getdata();
void *poll_proximity();

char acc_thread_exit;
char mag_thread_exit;
char light_thread_exit;
char orien_thread_exit;
char prox_thread_exit; //= 1;

static int orient_enabled = 0;
static int gyro_enabled = 0;

float last_gyro[3];

static Sensor_data sensor_data;

bool mFlushed = false;

/*pass values to kernel space*/
static int on = 1;
static int off = 0;

static int write_cmd(char const *path, char *cmd, int size)
{
	int fd, ret;
	char * mesg; 

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		mesg= strerror(errno);
		ALOGE("Meticulus: Cannot open %s, fd = %d, msg = %s\n", path, fd, mesg);
		return -ENODEV;
	}

	ret = write(fd, cmd, size);
	if (ret != size) {
		mesg= strerror(errno);
		ALOGE("Meticulus: path = %s\n", path);
		ALOGE("Meticulus: Error. Wrote: %d, should have written: %d, msg = %s\n", ret, size, strerror(errno));
	}

	close(fd);
	return ret;
}


/* implement individual sensor enable and disables */
static int activate_light(int enable)
{
	int ret = 0;
	int fd = -1;
	pthread_attr_t light_attr;
	pthread_t light_thread = -1;

	if (enable) {
		//if(DEBUG)
		//	ALOGD("%s: ========= count_light = %d, lightid = %d\n", __func__, count_light, light_id);
		if (count_light == 0) {
			//write_cmd(PATH_DELAY_LIGHT, "200\n", 4);
			write_cmd(PATH_POWER_LIGHT, "1\n", 2);
			light_thread_exit = 0;
			pthread_attr_init(&light_attr);
			/*
			 * Create thread in detached state, so that we
			 * need not join to clear its resources
			 */
			pthread_attr_setdetachstate(&light_attr,
					PTHREAD_CREATE_DETACHED);
			ret = pthread_create(&light_thread, &light_attr,
					&light_getdata, NULL);
			if(ret && DEBUG)
				ALOGD("could not create thread!");
			pthread_attr_destroy(&light_attr);
			count_light++;
		} else {
			count_light++;
		}
	} else {
		if (count_light == 0)
			return 0;
		count_light--;
		if (count_light == 0) {
			/*
			 * Enable acc_thread_exit to exit the thread
			 */
			light_thread_exit = 1;
			write_cmd(PATH_POWER_LIGHT, "0\n", 2);
		}
	}
	return ret;
}


/* implement individual sensor enable and disables */
static int activate_acc(int enable)
{
	int ret = 0;
	pthread_attr_t attr;
	pthread_t acc_thread = -1;
	int fd = -1;

	if (enable) {
		if(DEBUG)
			ALOGD("Meticulus: %s: ========= count_acc = %d, accid = %d\n", __func__, count_acc, acc_id);
		if (count_acc == 0) {
			write_cmd(PATH_POWER_ACC, "1",2);
			acc_thread_exit = 0;
			pthread_attr_init(&attr);
			/*
			 * Create thread in detached state, so that we
			 * need not join to clear its resources
			 */
			pthread_attr_setdetachstate(&attr,
					PTHREAD_CREATE_DETACHED);
			ret = pthread_create(&acc_thread, &attr,
					acc_getdata, NULL);
			pthread_attr_destroy(&attr);
			count_acc++;
		} else {
			count_acc++;
		}
	} else {
		if (count_acc == 0)
			return 0;
		count_acc--;
		if (count_acc == 0) {
			/*
			 * Enable acc_thread_exit to exit the thread
			 */
			acc_thread_exit = 1;
			write_cmd(PATH_POWER_ACC, "0",2);
		}
	}
	return ret;
}

static int activate_mag(int enable)
{
	int ret = 0;
	pthread_attr_t attr;
	pthread_t mag_thread = -1;
	//int fd = -1;

	if (enable) {
		if (count_mag == 0) {
			write_cmd(PATH_POWER_MAG, "1",2);
			mag_thread_exit = 0;
			pthread_attr_init(&attr);
			/*-
			 * Create thread in detached state, so that we
			 * need not join to clear its resources
			 */
			pthread_attr_setdetachstate(&attr,
					PTHREAD_CREATE_DETACHED);
			ret = pthread_create(&mag_thread, &attr,
					mag_getdata, NULL);
			pthread_attr_destroy(&attr);
			count_mag++;
		} else {
			count_mag++;
		}
	} else {
		if (count_mag == 0)
			return 0;
		count_mag--;
		if (count_mag == 0) {
			/*
			 * Enable mag_thread_exit to exit the thread
			 */
			mag_thread_exit = 1;
			write_cmd(PATH_POWER_MAG, "0",2);
		}
	}
	return ret;
}

static int activate_prox(int enable)
{
	int ret = -1;
	pthread_attr_t attr;
	pthread_t prox_thread = -1;

	if (enable) {
		if (count_prox == 0) {
			/*
			 * check for the file path
			 * Initialize prox_thread_exit flag
			 * every time thread is created
			 */
			write_cmd(PATH_POWER_PROX, "1",2);
			prox_thread_exit = 0;
			pthread_attr_init(&attr);
			/*
			 * Create thread in detached state, so that we
			 * need not join to clear its resources
			 */
			pthread_attr_setdetachstate(&attr,
					PTHREAD_CREATE_DETACHED);
			ret = pthread_create(&prox_thread, &attr,
					proximity_getdata, NULL);
			pthread_attr_destroy(&attr);
			count_prox++;
			/* Set an initial value */
			stprox_val.prox_flag = 1;
			stprox_val.prox_val = 1;
			poll_proximity();
		} else {
			count_prox++;
		}
	} else {
		if (count_prox == 0)
			return 0;
		count_prox--;
		if (count_prox == 0) {
			/*
			 * Enable prox_thread_exit to exit the thread
			 */
			prox_thread_exit = 1;
			write_cmd(PATH_POWER_PROX, "0",2);
			
		}
	}
	return 0;
}
static int poll_orientation(sensors_event_t *o, sensors_event_t *g)
{
	int fd_mag;
	int fd_acc;
	int data_mag[3];
	int data_acc[3];
	float reading[3];
	float gyro[3];
	float gain_mag[2] = {0.0};
	char buf[SIZE_OF_BUF];
	int nread;
	double mag_x, mag_y, mag_xy;
	double acc_x, acc_y, acc_z;

        //int numEventReceived = 0;
        int sensorId = HANDLE_ACCELEROMETER;
        while (mFlushed) {
            if (mFlushed & (1 << sensorId)) { /* Send flush META_DATA_FLUSH_COMPLETE immediately */
                sensors_event_t sensor_event;
                memset(&sensor_event, 0, sizeof(sensor_event));
                sensor_event.version = META_DATA_VERSION;
                sensor_event.type = SENSOR_TYPE_META_DATA;
                sensor_event.meta_data.sensor = sensorId;
                sensor_event.meta_data.what = 0;
                *o++ = sensor_event;
                //numEventReceived++;
                mFlushed &= ~(0x01 << sensorId);
                ALOGD_IF(DEBUG, "AkmSensor: %s Flushed sensorId: %d", __func__, sensorId);
            }
            sensorId++;
        }

	data_mag[0] = 0;
	data_mag[1] = 0;
	data_mag[2] = 0;

	data_acc[0] = 0;
	data_acc[1] = 0;
	data_acc[2] = 0;

	fd_acc = open(PATH_DATA_ACC , O_RDONLY);
	if (fd_acc < 0) {
		ALOGE("orient:Cannot open %s\n", PATH_DATA_ACC);
		return -ENODEV;
	}
	fd_mag = open(PATH_DATA_MAG, O_RDONLY);
	if (fd_mag < 0) {
		ALOGE("orien:Cannot open %s\n", PATH_DATA_MAG);
		return -ENODEV;
	}

	memset(buf, 0x00, sizeof(buf));
	lseek(fd_mag, 0, SEEK_SET);
	nread = read(fd_mag, buf, SIZE_OF_BUF);
	if (nread < 0) {
		ALOGE("orien:Error in reading data from Magnetometer\n");
		return -1;
	}
	sscanf(buf, "%d %d %d", &data_mag[0], &data_mag[1], &data_mag[2]);

	mag_x = (data_mag[0] * YAS530_RESOLUTION);
	mag_y = (data_mag[1] * YAS530_RESOLUTION);
	if (mag_x == 0) {
		if (mag_y < 0)
			reading[0] = 180;
		else
			reading[0] = 0;
	} else {
		mag_xy = mag_y / mag_x;
		if (mag_x > 0)
			reading[0] = round(270 +
						(atan(mag_xy) * RADIANS_TO_DEGREES));
		else
			reading[0] = round(90 +
						(atan(mag_xy) * RADIANS_TO_DEGREES));
	}

	memset(buf, 0x00, sizeof(buf));
	lseek(fd_acc, 0, SEEK_SET);
	nread = read(fd_acc, buf, SIZE_OF_BUF);
	if (nread < 0) {
		ALOGE("orient:Error in reading data from Accelerometer\n");
		return -1;
	}
	sscanf(buf, "%d %d %d", &data_acc[0], &data_acc[1], &data_acc[2]);

	acc_x = (float) data_acc[0];
	acc_x *= CONVERT_A;
	acc_y = (float) data_acc[1];
	acc_y *= CONVERT_A;
	acc_z = (float) data_acc[2];
	acc_z *= CONVERT_A;

	reading[1] = round(atan(acc_y / sqrt(acc_x*acc_x + acc_z*acc_z)) * RADIANS_TO_DEGREES);
	reading[2] = round(atan(acc_x / sqrt(acc_y*acc_y + acc_z*acc_z)) * RADIANS_TO_DEGREES);
	if(orient_enabled) {
	    o->sensor = HANDLE_ORIENTATION;
	    o->type = SENSOR_TYPE_ORIENTATION;
	    o->version = sizeof(struct sensors_event_t);
	    o->orientation.status = SENSOR_STATUS_ACCURACY_HIGH;
	    o->orientation.azimuth = reading[0];
	    o->orientation.pitch = reading[1];
	    o->orientation.roll = reading[2];
	}
	if(gyro_enabled) {
	    gyro[0] = reading[0] * DEGREES_TO_RADIANS;
	    gyro[1] = reading[1] * DEGREES_TO_RADIANS;
	    gyro[2] = reading[2] * DEGREES_TO_RADIANS;

	    g->sensor = HANDLE_GYROSCOPE;
	    g->type = SENSOR_TYPE_GYROSCOPE;
	    g->version = sizeof(struct sensors_event_t);
	    g->orientation.status = SENSOR_STATUS_ACCURACY_HIGH;
	    g->gyro.x = (gyro[0] - last_gyro[0]);
	    g->gyro.y = (gyro[1] - last_gyro[1]);
	    g->gyro.z = (gyro[2] - last_gyro[2]);

	    last_gyro[0] = gyro[0];
	    last_gyro[1] = gyro[1];
	    last_gyro[2] = gyro[2];

	}

	close(fd_acc);
	close(fd_mag);
	return 0;
}

/*
 * Check if same sensor type is already existing in the queue,
 * if so update the element or add at the end of queue.
 */
void add_queue(int sensor_type, sensors_event_t data)
{
	int i;
	pthread_mutex_lock(&sensordata_mutex);
	for (i = 0; i < stsensor_msgqueue.length; i++) {
		if (stsensor_msgqueue.sensor_data[i].sensor == sensor_type) {
			stsensor_msgqueue.sensor_data[i] = data;
			pthread_mutex_unlock(&sensordata_mutex);
			return;
		}
	}
	stsensor_msgqueue.sensor_data[stsensor_msgqueue.length] = data;
	stsensor_msgqueue.length++;
	/* signal event to mpoll if this is the first element in queue */
	if (stsensor_msgqueue.length == 1)
		pthread_cond_signal(&data_available_cv);
	pthread_mutex_unlock(&sensordata_mutex);
	return;
}

void *orient_getdata()
{
	sensors_event_t orient;
	sensors_event_t gyro;
	int ret;

	while (!orient_thread_exit) {
		usleep(delay_orient);
		ret = poll_orientation(&orient, &gyro);
		/* If return value = 0 queue the element */
		if (ret)
			return NULL;
		if(orient_enabled)
		    add_queue(HANDLE_ORIENTATION, orient);
		if(gyro_enabled)
		    add_queue(HANDLE_ORIENTATION, gyro);
	}
	return NULL;
}

static int activate_orientation(int enable)
{
	int ret = -1;
	pthread_attr_t attr;
	pthread_t orient_thread = -1;
 	if (enable) {
		if (count_orient == 0) {
			activate_acc(enable);
			activate_mag(enable);
			/*
			 * check for the file path
			 * Initialize prox_thread_exit flag
			 * every time thread is created
			 */
			orient_thread_exit = 0;
			pthread_attr_init(&attr);
			/*
			 * Create thread in detached state, so that we
			 * need not join to clear its resources
			 */
			pthread_attr_setdetachstate(&attr,
					PTHREAD_CREATE_DETACHED);
			ret = pthread_create(&orient_thread, &attr,
					orient_getdata, NULL);
			pthread_attr_destroy(&attr);
			count_orient++;
		} else {
			count_orient++;
		}
	} else {
		if (count_orient == 0)
			return 0;
		count_orient--;
		if (count_orient == 0) {
			/*
			 * Enable prox_thread_exit to exit the thread
			 */
			orient_thread_exit = 1;
			activate_acc(enable);
			activate_mag(enable);
		}
	}

	return 0;
}

static int poll_accelerometer(sensors_event_t *values)
{
	int fd;
	int nread;
	int data[3];
	char buf[SIZE_OF_BUF];

        //int numEventReceived = 0;
        int sensorId = HANDLE_ACCELEROMETER;
        while (mFlushed) {
            if (mFlushed & (1 << sensorId)) { /* Send flush META_DATA_FLUSH_COMPLETE immediately */
                sensors_event_t sensor_event;
                memset(&sensor_event, 0, sizeof(sensor_event));
                sensor_event.version = META_DATA_VERSION;
                sensor_event.type = SENSOR_TYPE_META_DATA;
                sensor_event.meta_data.sensor = sensorId;
                sensor_event.meta_data.what = 0;
                *values++ = sensor_event;
                //numEventReceived++;
                mFlushed &= ~(0x01 << sensorId);
                ALOGD_IF(DEBUG, "AkmSensor: %s Flushed sensorId: %d", __func__, sensorId);
            }
            sensorId++;
        }

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

	fd = open(PATH_DATA_ACC, O_RDONLY);
	if (fd < 0) {
		ALOGE("Meticulus: Cannot open %s\n", PATH_DATA_ACC);
		return -ENODEV;
	}

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, SIZE_OF_BUF);
	//ALOGE("ACC_POLL: %s", buf);
	if (nread < 0) {
		ALOGE("Meticulus: Error in reading data from accelerometer\n");
		return -1;
	}
	sscanf(buf, "%d,%d,%d", &data[0], &data[1], &data[2]);

	values->acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;
	values->acceleration.x = (float) (data[0]) * 9.81f;
	values->acceleration.x *= CONVERT_A;
	values->acceleration.y = (float) (data[1]) * 9.81f;
	values->acceleration.y *= CONVERT_A;
	values->acceleration.z = (float) (data[2]) * 9.81f;
	values->acceleration.z *= CONVERT_A;

	values->type = SENSOR_TYPE_ACCELEROMETER;
	values->sensor = HANDLE_ACCELEROMETER;
	values->version = sizeof(struct sensors_event_t);

	close(fd);
	return 0;
}

void *acc_getdata()
{
	sensors_event_t data;
	int ret;

	while (!acc_thread_exit) {
		usleep(delay_acc);
		ret = poll_accelerometer(&data);
		/* If return value = 0 queue the element */
		if (ret)
			return NULL;
		add_queue(HANDLE_ACCELEROMETER, data);
	}
	return NULL;
}

static int poll_magnetometer(sensors_event_t *values)
{
	int fd;
	int raw_data;
	int data[3];
	char buf[SIZE_OF_BUF];
	int nread;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

        //int numEventReceived = 0;
        int sensorId = HANDLE_ACCELEROMETER;
        while (mFlushed) {
            if (mFlushed & (1 << sensorId)) { /* Send flush META_DATA_FLUSH_COMPLETE immediately */
                sensors_event_t sensor_event;
                memset(&sensor_event, 0, sizeof(sensor_event));
                sensor_event.version = META_DATA_VERSION;
                sensor_event.type = SENSOR_TYPE_META_DATA;
                sensor_event.meta_data.sensor = sensorId;
                sensor_event.meta_data.what = 0;
                *values++ = sensor_event;
                //numEventReceived++;
                mFlushed &= ~(0x01 << sensorId);
                ALOGD_IF(DEBUG, "AkmSensor: %s Flushed sensorId: %d", __func__, sensorId);
            }
            sensorId++;
        }

	fd = open(PATH_DATA_MAG, O_RDONLY);
	if (fd < 0) {
		ALOGE("Meticulus: Cannot open %s\n", PATH_DATA_MAG);
		return -ENODEV;
	}

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, SIZE_OF_BUF);
	if (nread < 0) {
		ALOGE("Meticulus: Error in reading data from magnetometer\n");
		return -1;
	}
	sscanf(buf, "%d %d %d", &data[0], &data[1], &data[2]);
	close(fd);
        //raw_data *= YAS530_RESOLUTION;
	data[0] *= 0.001f;
	data[1] *= 0.001f;
	data[2] *= 0.001f;
	//ALOGE("Mag data: (%d %d %d)", data[0], data[1], data[2]);

	values->magnetic.status = SENSOR_STATUS_ACCURACY_HIGH;
	values->magnetic.x = (data[0]);
	values->magnetic.y = (data[1]);
	values->magnetic.z = (data[2]);
	values->sensor = HANDLE_MAGNETIC_FIELD;
	values->type = SENSOR_TYPE_MAGNETIC_FIELD;
	values->version = sizeof(struct sensors_event_t);

	return 0;
}

void *mag_getdata()
{
	sensors_event_t data;
	int ret;

	while (!mag_thread_exit) {
		usleep(delay_mag);
		ret = poll_magnetometer(&data);
		/* If return value = 0 queue the element */
		if (ret)
			return NULL;
		add_queue(HANDLE_MAGNETIC_FIELD, data);
	}
	return NULL;
}

static int poll_light(sensors_event_t *values)
{
	int fd;
	int data[3];
	char buf[SIZE_OF_BUF];
	int nread;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

	fd = open(PATH_DATA_LIGHT, O_RDONLY);
	if (fd < 0) {
		ALOGE("Cannot open %s\n", PATH_DATA_MAG);
		return -ENODEV;
	}

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, SIZE_OF_BUF);
	if (nread < 0) {
		ALOGE("Error in reading data from light sensor\n");
		return -1;
	}
	sscanf(buf, "%d %d %d", &data[0], &data[1], &data[2]);
	close(fd);

        values->light = data[0];
	values->sensor = HANDLE_LIGHT;
	values->type = SENSOR_TYPE_LIGHT;
	values->version = sizeof(struct sensors_event_t);

	return 0;
}

void *light_getdata()
{
	sensors_event_t data;
	int ret;

	while (!light_thread_exit) {
		//usleep(delay_light);
		ret = poll_light(&data);
		/* If return value = 0 queue the element */
		if (ret)
			return NULL;
		add_queue(HANDLE_LIGHT, data);
	}
	return NULL;
}

void *poll_proximity()
{
	int ret;
	sensors_event_t values;

	if(stprox_val.prox_flag)
	{
		stprox_val.prox_flag = 0;
		 /* normalize the distance */
		 if (stprox_val.prox_val == 0)
			values.distance = 0.0f;
		 else
			values.distance = 5.0f;
		 values.sensor = HANDLE_PROXIMITY;
		 values.type = SENSOR_TYPE_PROXIMITY;
		 values.version = sizeof(struct sensors_event_t);
		 add_queue(HANDLE_PROXIMITY, values);
		 
		 continue_next = 0;
		 events = 1;
	} else {
		continue_next = 1;
		if ((count_delay_sensors == 0) && (count_open_sensors > 0))
			events = 0;
	}

	/*
	 * events should be immediate but
	 * hardware & os drivers should support it.
	 * let give chnace to self disable and reading
	 * other sensors
	 */
	usleep(200000);
	return NULL;
}


void *proximity_getdata()
{
	int fd = -1,retval = -1;
	fd_set read_set;
	struct input_event ev;
	struct timeval tv;
	int size = sizeof(struct input_event);
	/* Initialize the structures */
	memset(&ev, 0x00, sizeof(ev));
	memset(&tv, 0x00, sizeof(tv));

	/* open input device */
	if ((fd = open(PATH_INTR_PROX, O_RDONLY)) > 0) {
		while(!prox_thread_exit) {
			/* Intialize the read descriptor */
			FD_ZERO(&read_set);
			FD_SET(fd,&read_set);
			/* Wait up to 0.5 seconds. */
			tv.tv_sec = 0 ;
			tv.tv_usec = 500000;
			retval = select(fd+1, &read_set, NULL, NULL, &tv);
			if (retval > 0) {
				/* FD_ISSET(0, &rfds) will be true. */
				if (FD_ISSET(fd, &read_set)) {
					read(fd, &ev, size );
					if(ev.type ==  EV_ABS) {
						//pthread_mutex_lock( &mutex_proxval );
						switch (ev.code) {
							case ABS_DISTANCE:
								stprox_val.prox_flag = 1;
								stprox_val.prox_val = ev.value;
								poll_proximity();
								break;

							default:								
								ALOGE("Meticulus: Got unsupported event: code 0x%x, value %d\n", ev.code, ev.value);
								break;
						}
						pthread_mutex_unlock( &mutex_proxval );
					} else {
						ALOGE("Meticulus - unsupported event type %d\n",ev.type);
					}
				}
			}
		}
		close(fd);
	}
	else
	if(DEBUG)
		ALOGD("Meticulus: %s is not a valid device\n", PATH_INTR_PROX);
	return NULL;
}

static int m_open_sensors(const struct hw_module_t *module,
		const char *name, struct hw_device_t **device);

static int m_sensors_get_sensors_list(struct sensors_module_t *module,
		struct sensor_t const **list)
{
	*list = sSensorList;

	return sizeof(sSensorList) / sizeof(sSensorList[0]);
}

static struct hw_module_methods_t m_sensors_module_methods = {
	.open = m_open_sensors
};

struct sensors_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = SENSORS_HARDWARE_MODULE_ID,
		.name = "Codinalte Sensors Module",
		.author = "Jonathan Jason Dennis [Meticulus]",
		.methods = &m_sensors_module_methods,
	},
	.get_sensors_list = m_sensors_get_sensors_list
};

/* enable and disable sensors here */
static int m_poll_activate(sensors_poll_device_1_t *dev,
		int handle, int enabled)
{
	int status = 0;
	int count_open_sensors = 0;
	if(DEBUG)
		ALOGD("Meticulus: Entering function %s with handle = %d,"
				" enable = %d\n", __FUNCTION__, handle, enabled);

	switch (handle) {
	case HANDLE_ORIENTATION:
		orient_enabled = enabled;
		status = activate_orientation(enabled);
		break;
	case HANDLE_ACCELEROMETER:
		status = activate_acc(enabled);
		break;
	case HANDLE_LIGHT:
		status = activate_light(enabled);
		break;
	case HANDLE_MAGNETIC_FIELD:
		status = activate_mag(enabled);
		break;
	case HANDLE_PROXIMITY:
		status = activate_prox(enabled);
		break;
	case HANDLE_GYROSCOPE:
		status = activate_orientation(enabled);
		gyro_enabled = enabled;
		break;
	default:
		if(DEBUG)
			ALOGD("Meticulus: This sensor/handle is not supported %s\n",
					__FUNCTION__);
		break;
	}

	/* check if sensor is missing then exit gracefully */
	if (status != -ENODEV) {
		/* count total number of sensors open */
		count_open_sensors = count_acc + count_mag  +
					count_prox + count_orien;
		if (count_open_sensors == 0) {
			pthread_mutex_lock(&sensordata_mutex);
			stsensor_msgqueue.length = 0;
			pthread_mutex_unlock(&sensordata_mutex);
		}
	}

	return 0;
}

static int set_delay_acc(int microseconds)
{
	int ret = -1;
	int fd = -1;
	int milliseconds = microseconds / 1000;
	fd = open(PATH_IO_ALPS, O_WRONLY);
	if(fd >= 0){
		ret = ioctl(fd,ALPSIO_SET_DELAY,&milliseconds);
		close(fd);
	}
	return ret;
}

static int set_delay_mag(int microseconds)
{
	/* Meticulus: alps sets the mag and acc delay together */
	return set_delay_acc(microseconds);
}

static int m_poll_set_delay(sensors_poll_device_1_t *dev,
		int handle, int64_t ns)
{
	int microseconds = ns / 1000;
	int ret = 0;
	if(DEBUG)
		ALOGD("Meticulus: set delay = %d in microseconds for handle = %d\n"
								, microseconds, handle);

	switch (handle) {
	case HANDLE_ORIENTATION:
		if (microseconds >= MINDELAY_ORIENTATION) {
			delay_orient = microseconds;
			ret = set_delay_acc(microseconds);
		}
		break;
	case HANDLE_ACCELEROMETER:
		if (microseconds >= MINDELAY_ACCELEROMETER) {
			delay_acc = microseconds;
			ret = set_delay_acc(microseconds);
		}
		break;
	case HANDLE_MAGNETIC_FIELD:
		if (microseconds >= MINDELAY_MAGNETIC_FIELD) {
			delay_mag = microseconds;
			ret = set_delay_mag(microseconds);
		}
		break;
	case HANDLE_PROXIMITY:
	case HANDLE_LIGHT:
		/* ignored */
		break;
	default:
		if(DEBUG)
			ALOGD("Meticulus libsensors:This sensor/handle is not supported %s\n",
					__FUNCTION__);
		break;
	}
	if (ret < 0)
		return -1;
	else
		return 0;
}


static int m_poll(sensors_poll_device_1_t *dev,
		sensors_event_t *data, int count)
{
	int i;
	struct timeval time;
	int events = 0;

	pthread_mutex_lock(&sensordata_mutex);
	/* If there are no elements in the queue
	 * wait till queue gets filled
	 */
	if (!stsensor_msgqueue.length)
		pthread_cond_wait(&data_available_cv, &sensordata_mutex);
	memcpy(data, &stsensor_msgqueue.sensor_data[0] ,
			sizeof(stsensor_msgqueue.sensor_data[0]));
	if (stsensor_msgqueue.length > 1) {
		for (i = 0; i < stsensor_msgqueue.length - 1; i++)
			memcpy(&stsensor_msgqueue.sensor_data[i],
			&stsensor_msgqueue.sensor_data[i+1],
			sizeof(stsensor_msgqueue.sensor_data[0]));
	}
	if(stsensor_msgqueue.length > 0)
		stsensor_msgqueue.length--;
	events = 1;
	pthread_mutex_unlock(&sensordata_mutex);
	/* add time stamp on last event */
	gettimeofday(&time, NULL);
	data->timestamp = (time.tv_sec * 1000000000LL) +
					(time.tv_usec * 1000);
	return events;
}

static int m_poll_batch(struct sensors_poll_device_1 *dev, int handle,
        int flags, int64_t period_ns, int64_t timeout) {
	(void)flags;
	(void)timeout;
	m_poll_set_delay(dev, handle, period_ns);
	return 0;
}

static int m_poll_flush(struct sensors_poll_device_1 *dev, int handle) {
    (void)dev;
    mFlushed |= (1 << handle);
    ALOGD("%s: handle: %d", __func__, handle);

    return 0;
}

/* close instace of the deevie */
static int m_poll_close(struct hw_device_t *dev)
{
	sensors_poll_device_1_t *poll_device =
		(sensors_poll_device_1_t *) dev;
	if(DEBUG)
		ALOGD("Meticulus: Closing poll data context.\n");

	pthread_cond_destroy(&data_available_cv);
	pthread_mutex_destroy(&sensordata_mutex);

	if (poll_device)
		free(poll_device);
	return 0;
}

/* open a new instance of a sensor device using name */
static int m_open_sensors(const struct hw_module_t *module,
		const char *name, struct hw_device_t **device)
{
	if(DEBUG)
		ALOGD("Meticulus: Entering function %s with param name = %s\n",
			__FUNCTION__, name);

	int status = -EINVAL;

	if (!strcmp(name, SENSORS_HARDWARE_POLL)) {
		sensors_poll_device_1_t *poll_device;
		poll_device = malloc(sizeof(*poll_device));
		if (!poll_device)
			return status;
		memset(poll_device, 0, sizeof(*poll_device));
		poll_device->common.tag = HARDWARE_DEVICE_TAG;
		poll_device->common.version = SENSORS_DEVICE_API_VERSION_1_3;
		poll_device->common.module = (struct hw_module_t *) module;
		poll_device->common.close = m_poll_close;
		poll_device->activate = m_poll_activate;
		poll_device->setDelay = m_poll_set_delay;
		poll_device->batch = m_poll_batch;
		poll_device->flush = m_poll_flush;
		poll_device->poll = m_poll;
		*device = &poll_device->common;

		pthread_cond_init(&data_available_cv, NULL);
		pthread_mutex_init(&sensordata_mutex, NULL);

		status = 0;
	}
	return status;
}
