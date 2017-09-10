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
#include "bricklib/drivers/twi/twi.h"

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

uint8_t DS2482_getAddress(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
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

// --------------------------------------------------------------------------------

// Helper functions to make dealing with I2C side easier
void DS2482_beginAndWrite(uint8_t byteToWrite)
{
    if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
        BA->bricklet_select(BS->port - 'a');
        TWI_StartWrite(BA->twid, DS2482_getAddress(), NULL, NULL, byteToWrite);
        while(!TWI_ByteSent(BA->twid));
	}
}

uint8_t DS2482_end()
{
    TWI_Stop(BA->twid);

    while(!TWI_TransferComplete(BA->twid));

    BA->bricklet_deselect(BS->port - 'a');
    BA->mutex_give(*BA->mutex_twi_bricklet);
}

void DS2482_writeByte(uint8_t data)
{
    TWI_WriteByte(BA->twid, data);
    while(!TWI_ByteSent(BA->twid));
}

uint8_t DS2482_readByte()
{
    if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
        uint8_t value;

        BA->bricklet_select(BS->port - 'a');
        BA->TWID_Read(BA->twid, DS2482_getAddress(), NULL, NULL, &value, 1, NULL);

        BA->bricklet_deselect(BS->port - 'a');
        BA->mutex_give(*BA->mutex_twi_bricklet);

        return value;
    }

    return 0;
}

/*// Simply starts and ends an Wire transmission
// If no devices are present, this returns false
uint8_t DS2482_checkPresence()
{
	begin();
	return !end() ? true : false;
}

// Performs a global reset of device state machine logic. Terminates any ongoing 1-Wire communication.
void DS2482_deviceReset()
{
	begin();
	write(DS2482_COMMAND_RESET);
	end();
}

// Sets the read pointer to the specified register. Overwrites the read pointer position of any 1-Wire communication command in progress.
void DS2482_setReadPointer(uint8_t readPointer)
{
	begin();
	writeByte(DS2482_COMMAND_SRP);
	writeByte(readPointer);
	end();
}

// Read the status register
uint8_t DS2482_readStatus()
{
	setReadPointer(DS2482_POINTER_STATUS);
	return readByte();
}

// Read the data register
uint8_t DS2482_readData()
{
	setReadPointer(DS2482_POINTER_DATA);
	return readByte();
}

// Read the config register
uint8_t DS2482_readConfig()
{
	setReadPointer(DS2482_POINTER_CONFIG);
	return readByte();
}

void DS2482_setStrongPullup()
{
	writeConfig(readConfig() | DS2482_CONFIG_SPU);
}

void DS2482_clearStrongPullup()
{
	writeConfig(readConfig() & !DS2482_CONFIG_SPU);
}

// Churn until the busy bit in the status register is clear
uint8_t DS2482_waitOnBusy()
{
	uint8_t status;

	for(int i=1000; i>0; i--)
	{
		status = readStatus();
		if (!(status & DS2482_STATUS_BUSY))
			break;
		delayMicroseconds(20);
	}

	// if we have reached this point and we are still busy, there is an error
	if (status & DS2482_STATUS_BUSY)
		mError = DS2482_ERROR_TIMEOUT;

	// Return the status so we don't need to explicitly do it again
	return status;
}

// Write to the config register
void DS2482_writeConfig(uint8_t config)
{
	waitOnBusy();
	begin();
	writeByte(DS2482_COMMAND_WRITECONFIG);
	// Write the 4 bits and the complement 4 bits
	writeByte(config | (~config)<<4);
	end();
	
	// This should return the config bits without the complement
	if (readByte() != config)
		mError = DS2482_ERROR_CONFIG;
}*/

// Generates a 1-Wire reset/presence-detect cycle (Figure 4) at the 1-Wire line. The state
// of the 1-Wire line is sampled at tSI and tMSP and the result is reported to the host 
// processor through the Status Register, bits PPD and SD.
/*uint8_t DS2482_wireReset()
{
	waitOnBusy();
	// Datasheet warns that reset with SPU set can exceed max ratings
	clearStrongPullup();

	waitOnBusy();

	begin();
	writeByte(DS2482_COMMAND_RESETWIRE);
	end();

	uint8_t status = waitOnBusy();

	if (status & DS2482_STATUS_SD)
	{
		mError = DS2482_ERROR_SHORT;
	}

	return (status & DS2482_STATUS_PPD) ? true : false;
}

// Writes a single data byte to the 1-Wire line.
void DS2482_wireWriteByte(uint8_t data, uint8_t power)
{
	waitOnBusy();
	if (power)
		setStrongPullup();
	begin();
	writeByte(DS2482_COMMAND_WRITEBYTE);
	writeByte(data);
	end();
}

// Generates eight read-data time slots on the 1-Wire line and stores result in the Read Data Register.
uint8_t DS2482_wireReadByte()
{
	waitOnBusy();
	begin();
	writeByte(DS2482_COMMAND_READBYTE);
	end();
	waitOnBusy();
	return readData();
}

// Generates a single 1-Wire time slot with a bit value “V” as specified by the bit byte at the 1-Wire line
// (see Table 2). A V value of 0b generates a write-zero time slot (Figure 5); a V value of 1b generates a 
// write-one time slot, which also functions as a read-data time slot (Figure 6). In either case, the logic
// level at the 1-Wire line is tested at tMSR and SBR is updated.
void DS2482_wireWriteBit(uint8_t data, uint8_t power)
{
	waitOnBusy();
	if (power)
		setStrongPullup();
	begin();
	writeByte(DS2482_COMMAND_SINGLEBIT);
	writeByte(data ? 0x80 : 0x00);
	end();
}

// As wireWriteBit
uint8_t DS2482_wireReadBit()
{
	wireWriteBit(1);
	uint8_t status = waitOnBusy();
	return status & DS2482_STATUS_SBR ? 1 : 0;
}

// 1-Wire skip
void DS2482_wireSkip()
{
	wireWriteByte(WIRE_COMMAND_SKIP);
}

void DS2482_wireSelect(const uint8_t rom[8])
{
	wireWriteByte(WIRE_COMMAND_SELECT);
	for (int i=0;i<8;i++)
		wireWriteByte(rom[i]);
}

//  1-Wire reset seatch algorithm
void DS2482_wireResetSearch()
{
	searchLastDiscrepancy = 0;
	searchLastDeviceFlag = 0;

	for (int i = 0; i < 8; i++)
	{
		searchAddress[i] = 0;
	}

}

// Perform a search of the 1-Wire bus
uint8_t DS2482_wireSearch(uint8_t *address)
{
	uint8_t direction;
	uint8_t last_zero=0;

	if (searchLastDeviceFlag)
		return 0;

	if (!wireReset())
		return 0;

	waitOnBusy();

	wireWriteByte(WIRE_COMMAND_SEARCH);

	for(uint8_t i=0;i<64;i++)
	{
		int searchByte = i / 8; 
		int searchBit = 1 << i % 8;

		if (i < searchLastDiscrepancy)
			direction = searchAddress[searchByte] & searchBit;
		else
			direction = i == searchLastDiscrepancy;

		waitOnBusy();
		begin();
		writeByte(DS2482_COMMAND_TRIPLET);
		writeByte(direction ? 0x80 : 0x00);
		end();

		uint8_t status = waitOnBusy();

		uint8_t id = status & DS2482_STATUS_SBR;
		uint8_t comp_id = status & DS2482_STATUS_TSB;
		direction = status & DS2482_STATUS_DIR;

		if (id && comp_id)
		{
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
		searchLastDeviceFlag = 1;

	for (uint8_t i=0; i<8; i++)
		address[i] = searchAddress[i];

	return 1;
}*/