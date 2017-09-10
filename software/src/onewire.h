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

#define FID_GET_NEXT_ADDRESS 1
#define MAX_DEVICE_ADDRESSES 1

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetDeviceAddresses;

typedef struct {
	MessageHeader header;
	uint8_t addresses[8];
} __attribute__((__packed__)) GetDeviceAddressesReturn;

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

void get_device_addresses(const ComType com, const GetDeviceAddresses *data);

void DS2482_setReadPtr(uint8_t readPtr);
uint8_t DS2482_readByte();
bool DS2482_readBytes(uint8_t* data, uint8_t length);
bool DS2482_writeByte(uint8_t data);
bool DS2482_writeBytes(uint8_t *data, uint8_t length);
uint8_t DS2482_get_address(void);

/*#define PTR_STATUS 0xf0
#define PTR_READ 0xe1
#define PTR_CONFIG 0xc3

#define DS2482_CONFIG_APU (1<<0)
#define DS2482_CONFIG_PPM (1<<1)
#define DS2482_CONFIG_SPU (1<<2)
#define DS2484_CONFIG_WS  (1<<3)

#define DS2482_STATUS_BUSY 	(1<<0)
#define DS2482_STATUS_PPD 	(1<<1)
#define DS2482_STATUS_SD	(1<<2)
#define DS2482_STATUS_LL	(1<<3)
#define DS2482_STATUS_RST	(1<<4)
#define DS2482_STATUS_SBR	(1<<5)
#define DS2482_STATUS_TSB	(1<<6)
#define DS2482_STATUS_DIR	(1<<7)

void DS2482_setReadPtr(uint8_t readPtr);
uint8_t DS2482_wireReadStatus(bool setPtr);
uint8_t DS2482_busyWait(bool setReadPtr);
void DS2482_reset();
bool DS2482_configure(uint8_t config);
bool DS2482_wireReset();
void DS2482_wireWriteByte(uint8_t b);
uint8_t DS2482_wireReadByte();
void DS2482_wireWriteBit(uint8_t bit);
void DS2482_wireResetSearch();
uint8_t DS2482_wireSearch(uint8_t *newAddr);*/


#define DS2482_COMMAND_RESET		0xF0	// Device reset

#define DS2482_COMMAND_SRP			0xE1 	// Set read pointer
	#define DS2482_POINTER_STATUS		0xF0
		#define DS2482_STATUS_BUSY			(1<<0)
		#define DS2482_STATUS_PPD			(1<<1)
		#define DS2482_STATUS_SD			(1<<2)
		#define DS2482_STATUS_LL			(1<<3)
		#define DS2482_STATUS_RST 			(1<<4)
		#define DS2482_STATUS_SBR			(1<<5)
		#define DS2482_STATUS_TSB 			(1<<6)
		#define DS2482_STATUS_DIR 			(1<<7)
	#define DS2482_POINTER_DATA			0xE1
	#define DS2482_POINTER_CONFIG		0xC3
		#define DS2482_CONFIG_APU			(1<<0)
		#define DS2482_CONFIG_SPU			(1<<2)
		#define DS2482_CONFIG_1WS			(1<<3)


#define DS2482_COMMAND_WRITECONFIG	0xD2
#define DS2482_COMMAND_RESETWIRE	0xB4
#define DS2482_COMMAND_WRITEBYTE	0xA5
#define DS2482_COMMAND_READBYTE		0x96
#define DS2482_COMMAND_SINGLEBIT	0x87
#define DS2482_COMMAND_TRIPLET		0x78

#define WIRE_COMMAND_SKIP			0xCC
#define WIRE_COMMAND_SELECT			0x55
#define WIRE_COMMAND_SEARCH			0xF0

#define DS2482_ERROR_TIMEOUT		(1<<0)
#define DS2482_ERROR_SHORT			(1<<1)
#define DS2482_ERROR_CONFIG			(1<<2)

void DS2482_deviceReset();
void DS2482_setReadPointer(uint8_t readPointer);
uint8_t DS2482_readStatus();
uint8_t DS2482_waitOnBusy();
uint8_t DS2482_wireReset();
void DS2482_wireResetSearch();
uint8_t DS2482_wireSearch(uint8_t *address);
void DS2482_wireWriteByte(uint8_t data);
void DS2482_writeConfig(uint8_t config);
uint8_t DS2482_readConfig();
void DS2482_clearStrongPullup();

#endif
