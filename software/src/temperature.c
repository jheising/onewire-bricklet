/* temperature-bricklet
 * Copyright (C) 2010-2012 Olaf Lüke <olaf@tinkerforge.com>
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

#include "temperature.h"

#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "config.h"

#define I2C_EEPROM_ADDRESS_HIGH 84

#define I2C_ADDRESS_HIGH 73 // 0b1001001
#define I2C_ADDRESS_LOW 72 // 0b1001000
#define I2C_INTERNAL_ADDRESS_TEMPERATURE 0x0
#define I2C_INTERNAL_ADDRESS_BYTES 1
#define I2C_DATA_LENGTH 2

// Temperature is defined by a multiplier of 0.0625, thus means we get
// 100th degree if we do value*100/16
#define TEMP_SCALE_MUL 100
#define TEMP_SCALE_DIV 16

#define SIMPLE_UNIT_TEMPERATURE 0

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_TEMPERATURE
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_TEMPERATURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_TEMPERATURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_TEMPERATURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{get_temperature, SIMPLE_SIGNEDNESS_INT, FID_TEMPERATURE, FID_TEMPERATURE_REACHED, SIMPLE_UNIT_TEMPERATURE} // temperature
};

const uint8_t smp_length = sizeof(smp);

void invocation(const ComType com, const uint8_t *data) {
	simple_invocation(com, data);

	if(((MessageHeader*)data)->fid > FID_LAST) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
	}
}

void constructor(void) {
	simple_constructor();
}

void destructor(void) {
	simple_destructor();
}

void tick(const uint8_t tick_type) {
	simple_tick(tick_type);
}

int32_t get_temperature(const int32_t value) {
	int32_t new_value = value;
    if((BC->tick % 250) == 0) {
    	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
    		new_value = temperature_read();
    		BA->mutex_give(*BA->mutex_twi_bricklet);
    	}
    }
    return new_value;
}

int16_t two_complement_12_to_16(const int16_t value) {
	if(value & (1 << 11)) {
		return value | 0xF000;
	}

	return value;
}

int16_t temperature_read(void) {
	const uint8_t port = BS->port - 'a';

	uint16_t value;

	BA->bricklet_select(port);

	Twi* twi = BA->twid->pTwi;

	// Start read at I2C_ADDRESS
	twi->TWI_MMR = 0;
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		twi->TWI_MMR = TWI_MMR_MREAD | (I2C_ADDRESS_HIGH << 16);
	} else {
		twi->TWI_MMR = TWI_MMR_MREAD | (I2C_ADDRESS_LOW << 16);
	}
	twi->TWI_IADR = 0;
	twi->TWI_CR = TWI_CR_START;

	// Read first byte
	while(!((twi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY));
	value = twi->TWI_RHR;

	// Read second byte
    twi->TWI_CR = TWI_CR_STOP;
	while(!((twi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY));
	value = (twi->TWI_RHR | (value << 8));

	while(!((twi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP));

	BA->bricklet_deselect(port);

	return two_complement_12_to_16(value >> 4)*TEMP_SCALE_MUL/TEMP_SCALE_DIV;
}
