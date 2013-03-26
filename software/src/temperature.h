/* temperature-bricklet
 * Copyright (C) 2010-2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * temperature.h: Implementation of Temperature Bricklet messages
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

#include "bricklib/com/com_common.h"

#define I2C_MODE_FAST 0
#define I2C_MODE_SLOW 1

#define FID_GET_TEMPERATURE 1
#define FID_SET_TEMPERATURE_CALLBACK_PERIOD 2
#define FID_GET_TEMPERATURE_CALLBACK_PERIOD 3
#define FID_SET_TEMPERATURE_CALLBACK_THRESHOLD 4
#define FID_GET_TEMPERATURE_CALLBACK_THRESHOLD 5
#define FID_SET_DEBOUNCE_PERIOD 6
#define FID_GET_DEBOUNCE_PERIOD 7
#define FID_TEMPERATURE 8
#define FID_TEMPERATURE_REACHED 9
#define FID_SET_I2C_MODE 10
#define FID_GET_I2C_MODE 11

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetI2CMode;

typedef struct {
	MessageHeader header;
	uint8_t mode;
} __attribute__((__packed__)) GetI2CModeReturn;

typedef struct {
	MessageHeader header;
	uint8_t mode;
} __attribute__((__packed__)) SetI2CMode;

void get_i2c_mode(const ComType com, const GetI2CMode *data);
void set_i2c_mode(const ComType com, const SetI2CMode *data);

int32_t get_temperature(const int32_t value);
int16_t two_complement_12_to_16(const int16_t value);
int16_t temperature_read(void);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif
