/*
 * Copyright (C) 2016 The CyanogenMod Project <http://www.cyanogenmod.org>
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
#include <hardware/gps.h>


/* 2G and 3G */
/* In 3G lac is discarded */
typedef struct {
    uint16_t type;
    uint16_t mcc;
    uint16_t mnc;
    uint16_t lac;
    uint16_t psc;
    uint32_t cid;
} AGpsRefLocationCellIDSamsung;

typedef struct {
    uint8_t mac[6];
} AGpsRefLocationMac;

/** Represents ref locations */
typedef struct {
    uint16_t type;
    union {
        AGpsRefLocationCellIDSamsung   cellID;
        AGpsRefLocationMac      mac;
    } u;
} AGpsRefLocationSamsung;


/* 2G and 3G */
/* In 3G lac is discarded */
typedef struct {
    uint16_t type;
    uint16_t mcc;
    uint16_t mnc;
    uint16_t lac;
    uint32_t cid;
} AGpsRefLocationCellID;

/** Represents ref locations */
typedef struct {
    uint16_t type;
    union {
        AGpsRefLocationCellID   cellID;
        AGpsRefLocationMac      mac;
    } u;
} AGpsRefLocation;
