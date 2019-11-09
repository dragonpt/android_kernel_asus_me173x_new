/*
 * Copyright (C) 2008 The Android Open Source Project
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
 
#ifndef __HWMSEN_CUSTOM_H__ 
#define __HWMSEN_CUSTOM_H__

#ifdef CUSTOM_KERNEL_ACCELEROMETER
	#define ACCELEROMETER 		"LIS3DHTR 3-axis Accelerometer"
	#define ACCELEROMETER_VENDER 	"ST"
#endif

#ifdef CUSTOM_KERNEL_ALSPS
	#define PROXIMITY 		"STK3310 Proximity Sensor"
	#define PROXIMITY_VENDER 	"SITRONIX"
	#define LIGHT 			"STK3310 Light Sensor"
	#define LIGHT_VENDER 		"SITRONIX"
#endif

#ifdef CUSTOM_KERNEL_MAGNETOMETER
	#define MAGNETOMETER 		"AKM8963C 3-axis Magnetic Field sensor"
	#define MAGNETOMETER_VENDER 	"AKM"
	#define ORIENTATION 		"AKM8963C Orientation sensor"
	#define ORIENTATION_VENDER 	"AKM"
#endif

#ifdef CUSTOM_KERNEL_GYROSCOPE
	#define GYROSCOPE 		"L3G4200DTR gyroscope Sensor"
	#define GYROSCOPE_VENDER 	"ST"
#endif

#ifdef CUSTOM_KERNEL_BAROMETER
	#define PRESSURE 		"BMA050"
	#define PRESSURE_VENDER		"BMA050"
#endif

#ifdef CUSTOM_KERNEL_TEMPURATURE
	#define TEMPURATURE 		"BMA050"
	#define TEMPURATURE_VENDER	"BMA050"
#endif

#endif

