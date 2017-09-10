/* temperature-bricklet
 * Copyright (C) 2010-2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * temperature.c: Implementation of Temperature Bricklet messages
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

#include "onewire.h"

#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "config.h"

#define I2C_EEPROM_ADDRESS_HIGH 84

#define I2C_ADDRESS_HIGH 25 // 0b0011001
#define I2C_ADDRESS_LOW 24 // 0b0011000


void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	DS2482_deviceReset();
	SLEEP_MS(5);
}

void destructor(void) {
}

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {

		case FID_GET_NEXT_ADDRESS:
		{
		    get_device_addresses(com, (GetDeviceAddresses*)data);
			break;
		}
		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void get_device_addresses(const ComType com, const GetDeviceAddresses *data) {
    GetDeviceAddressesReturn response;

    response.header         = data->header;
    response.header.length  = sizeof(GetDeviceAddressesReturn);
    memset(response.addresses, 0, 8);

    BA->send_blocking_with_timeout(&response, sizeof(GetDeviceAddressesReturn), com);
}

void tick(const uint8_t tick_type) {
}

// -----------------------------------------------


uint8_t DS2482_readByte()
{
    uint8_t theByte = 0;
    DS2482_readBytes(&theByte, 1);
    return theByte;
}

bool DS2482_readBytes(uint8_t* data, uint8_t length)
{
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {

        BA->bricklet_select(BS->port - 'a');
        BA->TWID_Read(BA->twid, DS2482_get_address(), 0x0, 0x0, data, length, NULL);
        BA->bricklet_deselect(BS->port - 'a');
        BA->mutex_give(*BA->mutex_twi_bricklet);

        return true;
    }

    return false;
}

bool DS2482_writeByte(uint8_t data)
{
    return DS2482_writeBytes(&data, 1);
}

bool DS2482_writeBytes(uint8_t *data, uint8_t length)
{
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {

        BA->bricklet_select(BS->port - 'a');
        BA->TWID_Write(BA->twid, DS2482_get_address(), 0x0, 0x0, data, length, NULL);
        BA->bricklet_deselect(BS->port - 'a');
        BA->mutex_give(*BA->mutex_twi_bricklet);

        return true;
    }

    return false;
}

uint8_t DS2482_get_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------

// Write to the config register
void DS2482_writeConfig(uint8_t config)
{
	//DS2482_waitOnBusy();
	DS2482_writeByte(DS2482_COMMAND_WRITECONFIG);
	// Write the 4 bits and the complement 4 bits
	DS2482_writeByte(config | (~config) << 4);

	// This should return the config bits without the complement
	if (DS2482_readByte() != config)
	{
	    BC->mError = DS2482_ERROR_CONFIG;
	}
}

// Read the config register
uint8_t DS2482_readConfig()
{
	DS2482_setReadPointer(DS2482_POINTER_CONFIG);
	return DS2482_readByte();
}

// Sets the read pointer to the specified register. Overwrites the read pointer position of any 1-Wire communication command in progress.
void DS2482_setReadPointer(uint8_t readPointer)
{
	DS2482_writeByte(DS2482_COMMAND_SRP);
	DS2482_writeByte(readPointer);
}

// Read the status register
uint8_t DS2482_readStatus()
{
	DS2482_setReadPointer(DS2482_POINTER_STATUS);
	return DS2482_readByte();
}

uint8_t DS2482_waitOnBusy()
{
	uint8_t status;

	for(int i=1000; i>0; i--)
	{
		status = DS2482_readStatus();
		if (!(status & DS2482_STATUS_BUSY))
			break;
		SLEEP_US(20);
	}

	// if we have reached this point and we are still busy, there is an error
	if (status & DS2482_STATUS_BUSY)
		BC->mError = DS2482_ERROR_TIMEOUT;

	// Return the status so we don't need to explicitly do it again
	return status;
}

void DS2482_deviceReset()
{
    BC->mError = 0;
	DS2482_writeByte(DS2482_COMMAND_RESET);
}

void DS2482_clearStrongPullup()
{
	DS2482_writeConfig(DS2482_readConfig() & !DS2482_CONFIG_SPU);
}

uint8_t DS2482_wireReset()
{
	DS2482_waitOnBusy();
	// Datasheet warns that reset with SPU set can exceed max ratings
	DS2482_clearStrongPullup();

	DS2482_waitOnBusy();

	DS2482_writeByte(DS2482_COMMAND_RESETWIRE);

	uint8_t status = DS2482_waitOnBusy();

	if (status & DS2482_STATUS_SD)
	{
		BC->mError = DS2482_ERROR_SHORT;
	}

	return (status & DS2482_STATUS_PPD) ? true : false;
}

// Writes a single data byte to the 1-Wire line.
void DS2482_wireWriteByte(uint8_t data)
{
	DS2482_waitOnBusy();
	//if (power)
	//	setStrongPullup();
	DS2482_writeByte(DS2482_COMMAND_WRITEBYTE);
	DS2482_writeByte(data);
}

uint8_t searchLastDeviceFlag = 0;
uint8_t searchAddress[8];
uint8_t searchLastDiscrepancy = 0;

//  1-Wire reset search algorithm
void DS2482_wireResetSearch()
{
	searchLastDiscrepancy = 0;
	searchLastDeviceFlag = 0;

	for (int i = 0; i < 8; i++)
	{
		searchAddress[i] = 0;
	}
}

uint8_t DS2482_wireSearch(uint8_t *address)
{
	uint8_t direction;
	uint8_t last_zero=0;

	if (searchLastDeviceFlag)
	{
	    address[0] = 21;
	    return 0;
	}


	if (!DS2482_wireReset())
	{
	    address[0] = 22;
	    return 0;
	}

	DS2482_waitOnBusy();

	DS2482_wireWriteByte(WIRE_COMMAND_SEARCH);

	for(uint8_t i=0;i<64;i++)
	{
		int searchByte = i / 8;
		int searchBit = 1 << i % 8;

		if (i < searchLastDiscrepancy)
			direction = searchAddress[searchByte] & searchBit;
		else
			direction = i == searchLastDiscrepancy;

		DS2482_waitOnBusy();
		DS2482_writeByte(DS2482_COMMAND_TRIPLET);
		DS2482_writeByte(direction ? 0x80 : 0x00);

		uint8_t status = DS2482_waitOnBusy();

		uint8_t id = status & DS2482_STATUS_SBR;
		uint8_t comp_id = status & DS2482_STATUS_TSB;
		direction = status & DS2482_STATUS_DIR;

		if (id && comp_id)
		{
		    address[0] = 23;
			return 0;
		}
		else
		{
			if (!id && !comp_id && !direction)
			{
				last_zero = i;
			}
		}

		if (direction)
			searchAddress[searchByte] |= searchBit;
		else
			searchAddress[searchByte] &= ~searchBit;

	}

	searchLastDiscrepancy = last_zero;

	if (!last_zero)
	{
	    searchLastDeviceFlag = 1;
	}

	for (uint8_t i=0; i<8; i++)
	{
	    address[i] = searchAddress[i];
	}

	return 1;
}

/*void DS2482_setReadPtr(uint8_t readPtr)
{

	DS2482_writeByte(0xe1);  // changed from 'send' to 'write' according http://blog.makezine.com/2011/12/01/arduino-1-0-is-out-heres-what-you-need-to-know/'
	DS2482_writeByte(readPtr);

}

uint8_t DS2482_wireReadStatus(bool setPtr)
{
	if (setPtr)
		DS2482_setReadPtr(PTR_STATUS);

	return DS2482_readByte();
}

uint8_t DS2482_busyWait(bool setReadPtr)
{
	uint8_t status;
	int loopCount = 1000;
	while((status = DS2482_wireReadStatus(setReadPtr)) & DS2482_STATUS_BUSY)
	{
		if (--loopCount <= 0)
		{
			BC->mTimeout = 1;
			break;
		}
		SLEEP_US(20);
	}
	return status;
}

//----------interface
void DS2482_reset()
{
	BC->mTimeout = 0;
	DS2482_writeByte(0xf0);
}

bool DS2482_configure(uint8_t config)
{
	DS2482_busyWait(true);

	DS2482_writeByte(0xd2);
	DS2482_writeByte(config | (~config)<<4);


	return DS2482_readByte() == config;
}

bool DS2482_wireReset()
{
	DS2482_busyWait(true);

	DS2482_writeByte(0xb4);

	uint8_t status = DS2482_busyWait(false);

	return status & DS2482_STATUS_PPD ? true : false;
}


void DS2482_wireWriteByte(uint8_t b)
{
	DS2482_busyWait(true);

	DS2482_writeByte(0xa5);
	DS2482_writeByte(b);
}

uint8_t DS2482_wireReadByte()
{
	DS2482_busyWait(true);

	DS2482_writeByte(0x96);

	DS2482_busyWait(false);
	DS2482_setReadPtr(PTR_READ);
	return DS2482_readByte();
}

void DS2482_wireWriteBit(uint8_t bit)
{
	DS2482_busyWait(true);

	DS2482_writeByte(0x87);
	DS2482_writeByte(bit ? 0x80 : 0);

}


void DS2482_wireResetSearch()
{
	BC->searchExhausted = 0;
	BC->searchLastDisrepancy = 0;

	for(uint8_t i = 0; i<8; i++)
		BC->searchAddress[i] = 0;
}

uint8_t DS2482_wireSearch(uint8_t *newAddr)
{
	uint8_t i;
	uint8_t direction;
	uint8_t last_zero=0;

	if (BC->searchExhausted)
		return 0;

	if (!DS2482_wireReset())
		return 0;

	DS2482_busyWait(true);
	DS2482_wireWriteByte(0xf0);

	for(i=1;i<65;i++)
	{
		int romByte = (i-1)>>3;
		int romBit = 1<<((i-1)&7);

		if (i < BC->searchLastDisrepancy)
			direction = BC->searchAddress[romByte] & romBit;
		else
			direction = i == BC->searchLastDisrepancy;

		DS2482_busyWait(false);

		DS2482_writeByte(0x78);
		DS2482_writeByte(direction ? 0x80 : 0);

		uint8_t status = DS2482_busyWait(false);

		uint8_t id = status & DS2482_STATUS_SBR;
		uint8_t comp_id = status & DS2482_STATUS_TSB;
		direction = status & DS2482_STATUS_DIR;

		if (id && comp_id)
			return 0;
		else
		{
			if (!id && !comp_id && !direction)
				last_zero = i;
		}

		if (direction)
			BC->searchAddress[romByte] |= romBit;
		else
			BC->searchAddress[romByte] &= (uint8_t)~romBit;
	}

	BC->searchLastDisrepancy = last_zero;

	if (last_zero == 0)
		BC->searchExhausted = 1;

	for (i=0;i<8;i++)
		newAddr[i] = BC->searchAddress[i];

	return 1;
}*/