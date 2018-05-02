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

#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdint.h>

#include "bricklib/com/com_common.h"

#define FID_WIRE_RESET 1
#define FID_WIRE_RESET_SEARCH 2
#define FID_WIRE_SEARCH 3
#define FID_WIRE_WRITE_BIT 4
#define FID_WIRE_READ_BIT 5
#define FID_WIRE_WRITE_BYTE 6
#define FID_WIRE_READ_BYTE 7
#define FID_WIRE_SELECT 8
#define FID_WIRE_SKIP 9
#define FID_SET_DS2482_CONFIG 10
#define FID_GET_DS2482_CONFIG 11
#define FID_RESET_DS2482 12

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) EmptyMessage;

typedef struct {
	MessageHeader header;
	uint8_t theByte;
} __attribute__((__packed__)) ByteMessage;

typedef struct {
	MessageHeader header;
	int16_t theTemp;
} __attribute__((__packed__)) TwoByteMessage;

typedef struct {
	MessageHeader header;
	uint8_t address[8];
} __attribute__((__packed__)) WireAddressMessage;

typedef struct {
	MessageHeader header;
	uint8_t address[8];
	uint8_t moreAvailable;
} __attribute__((__packed__)) WireSearchMessage;

typedef struct {
	MessageHeader header;
	uint8_t activePullup;
	uint8_t strongPullup;
	uint8_t speed;
} __attribute__((__packed__)) DS2482ConfigMessage;

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

void send_simple_response(const ComType com, const uint8_t* data);

#define WIRE_COMMAND_SKIP			0xCC
#define WIRE_COMMAND_SELECT			0x55
#define WIRE_COMMAND_SEARCH			0xF0

uint8_t wireReset();
void wireWriteByte(uint8_t data, uint8_t power);
uint8_t wireReadByte();
void wireWriteBit(uint8_t data, uint8_t power);
uint8_t wireReadBit();
void wireSkip();
void wireSelect(const uint8_t *rom);
void wireResetSearch();
uint8_t wireSearch(uint8_t *address);

#endif
