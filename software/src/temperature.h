/* temperature-bricklet
 * Copyright (C) 2010-2011 Olaf Lüke <olaf@tinkerforge.com>
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

#define TYPE_GET_TEMPERATURE 1
#define TYPE_SET_TEMPERATURE_CALLBACK_PERIOD 2
#define TYPE_GET_TEMPERATURE_CALLBACK_PERIOD 3
#define TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD 4
#define TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD 5
#define TYPE_SET_DEBOUNCE_PERIOD 6
#define TYPE_GET_DEBOUNCE_PERIOD 7
#define TYPE_TEMPERATURE 8
#define TYPE_TEMPERATURE_REACHED 9

int32_t get_temperature(int32_t value);
int16_t two_complement_12_to_16(const int16_t value);
int16_t temperature_read(void);

void invocation(uint8_t com, uint8_t *data);
void constructor(void);
void destructor(void);
void tick(void);

#endif
