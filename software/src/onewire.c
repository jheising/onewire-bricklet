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
#include "DS2482.h"

#include "bricklib/drivers/twi/twi.h"
#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "brickletlib/bricklet_debug.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "config.h"

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

    BC->conversionSleep = 0;
	DS2482_deviceReset();
	SLEEP_MS(5);
}

void destructor(void) {
}

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
	    case FID_WIRE_RESET:
        {
            wireReset();
            send_simple_response(com, data);
            break;
        }
        case FID_WIRE_RESET_SEARCH:
		{
		    wireResetSearch();
		    send_simple_response(com, data);
			break;
		}
		case FID_WIRE_SEARCH:
		{
            WireSearchMessage response;

            response.header         = ((EmptyMessage*)data)->header;
            response.header.length  = sizeof(WireSearchMessage);
            response.moreAvailable = 0;
            memset(response.address, 0, 8);

            response.moreAvailable = wireSearch(response.address);

            BA->send_blocking_with_timeout(&response, sizeof(WireSearchMessage), com);

			break;
		}
        case FID_WIRE_WRITE_BIT:
        {
            wireWriteBit(((ByteMessage*)data)->theByte, false);
            send_simple_response(com, data);
            break;
        }
        case FID_WIRE_READ_BIT:
        {
            ByteMessage returnMessage;

            returnMessage.header = ((EmptyMessage*)data)->header;
            returnMessage.header.length = sizeof(ByteMessage);
            returnMessage.theByte = wireReadBit();

            BA->send_blocking_with_timeout(&returnMessage, sizeof(ByteMessage), com);

            break;
        }
		case FID_WIRE_WRITE_BYTE:
        {
            wireWriteByte(((ByteMessage*)data)->theByte, false);
            send_simple_response(com, data);
            break;
        }
        case FID_WIRE_READ_BYTE:
        {
            ByteMessage returnMessage;

            returnMessage.header = ((EmptyMessage*)data)->header;
            returnMessage.header.length = sizeof(ByteMessage);
            returnMessage.theByte = wireReadByte();

            BA->send_blocking_with_timeout(&returnMessage, sizeof(ByteMessage), com);

            break;
        }
        case FID_WIRE_SELECT:
        {
            wireSelect(((WireAddressMessage*)data)->address);
            send_simple_response(com, data);
            break;
        }
        case FID_WIRE_SKIP:
        {
            wireSkip();
            send_simple_response(com, data);
            break;
        }
        case FID_SET_DS2482_CONFIG:
        {
            // TODO: Implement
            send_simple_response(com, data);
            break;
        }
        case FID_GET_DS2482_CONFIG:
        {
            // TODO: Implement
            send_simple_response(com, data);
            break;
        }
        case FID_RESET_DS2482:
        {
            DS2482_deviceReset();
            SLEEP_MS(5);
            send_simple_response(com, data);
        }
        case FID_TEMP_START_CONVERSION:
        {
            wireReset();
            wireSkip();
            wireWriteByte(0x44, false);
            send_simple_response(com, data);
            break;
        }
        case FID_TEMP_READ_SCRATCH:
        {
            wireReset();
            wireSelect(((WireAddressMessage*)data)->address);
            wireWriteByte(0xBE, false);

            TempScratchPadMessage response;
            response.header         = ((WireAddressMessage*)data)->header;
            response.header.length  = sizeof(TempScratchPadMessage);

            for(uint8_t i = 0; i < 9; i++){
                response.scratchPad[i] = wireReadByte();
            }

            wireReset();

            BA->send_blocking_with_timeout(&response, sizeof(TempScratchPadMessage), com);
        }
        case FID_TEMP_SET_RESOLUTION:
        {
            uint8_t resByte;
            int16_t conversionSleep;

            switch(((ByteMessage*)data)->theByte)
            {
                case 9:
                {
                    resByte = TEMP_9_BIT;
                    conversionSleep = 94;
                    break;
                }
                case 10:
                {
                    resByte = TEMP_10_BIT;
                    conversionSleep = 188;
                    break;
                }
                case 11:
                {
                    resByte = TEMP_11_BIT;
                    conversionSleep = 375;
                    break;
                }
                default:
                {
                    resByte = TEMP_12_BIT;
                    conversionSleep = 750;
                }
            }

            wireReset();
            wireSkip();
            wireWriteByte(WRITESCRATCH, false);
            wireWriteByte(0x0, false);
            wireWriteByte(0x0, false);
            wireWriteByte(resByte, false);
            wireReset();

            send_simple_response(com, data);
            break;
        }
		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void tick(const uint8_t tick_type) {
}

void send_simple_response(const ComType com, const uint8_t* data) {
    EmptyMessage response;
    response.header = ((EmptyMessage*)data)->header;
    response.header.length = sizeof(EmptyMessage);
    BA->send_blocking_with_timeout(&response, sizeof(EmptyMessage), com);
}

/*int16_t temp_set_resolution(const uint8_t address[8], const uint8_t resolution)
{
    uint8_t resByte;
    int16_t conversionSleep;

    switch(resolution)
    {
        case 9:
        {
            resByte = TEMP_9_BIT;
            conversionSleep = 94;
            break;
        }
        case 10:
        {
            resByte = TEMP_10_BIT;
            conversionSleep = 188;
            break;
        }
        case 11:
        {
            resByte = TEMP_11_BIT;
            conversionSleep = 375;
            break;
        }
        default:
        {
            resByte = TEMP_12_BIT;
            conversionSleep = 750;
        }
    }

    wireReset();

    if(address == NULL)
    {
        wireSkip();
    }
    else
    {
        wireSelect(address);
    }

    wireWriteByte(TEMP_WRITESCRATCH, false);
    wireWriteByte(0x0, false);
    wireWriteByte(0x0, false);
    wireWriteByte(resByte, false);

    return conversionSleep;
}
*/

// Generates a 1-Wire reset/presence-detect cycle (Figure 4) at the 1-Wire line. The state
// of the 1-Wire line is sampled at tSI and tMSP and the result is reported to the host 
// processor through the Status Register, bits PPD and SD.
uint8_t wireReset()
{
	//DS2482_waitOnBusy(true);
	// Datasheet warns that reset with SPU set can exceed max ratings
	//DS2482_clearStrongPullup();

	DS2482_waitOnBusy(true);

	DS2482_writeByte(DS2482_COMMAND_RESETWIRE);

	uint8_t status = DS2482_waitOnBusy(false);

	if (status & DS2482_STATUS_SD)
	{
		BC->mError = DS2482_ERROR_SHORT;
	}

	return status & DS2482_STATUS_PPD;
}

// Writes a single data byte to the 1-Wire line.
void wireWriteByte(uint8_t data, uint8_t power)
{
	DS2482_waitOnBusy(true);
	if (power)
		DS2482_setStrongPullup();
	DS2482_writeBytes((uint8_t[]){DS2482_COMMAND_WRITEBYTE, data}, 2);
}

// Generates eight read-data time slots on the 1-Wire line and stores result in the Read Data Register.
uint8_t wireReadByte()
{
	DS2482_waitOnBusy(true);
    DS2482_writeByte(DS2482_COMMAND_READBYTE);
	DS2482_waitOnBusy(false);
	return DS2482_readData();
}

// Generates a single 1-Wire time slot with a bit value “V” as specified by the bit byte at the 1-Wire line
// (see Table 2). A V value of 0b generates a write-zero time slot (Figure 5); a V value of 1b generates a 
// write-one time slot, which also functions as a read-data time slot (Figure 6). In either case, the logic
// level at the 1-Wire line is tested at tMSR and SBR is updated.
void wireWriteBit(uint8_t data, uint8_t power)
{
	DS2482_waitOnBusy(true);

	if (power)
		DS2482_setStrongPullup();

    DS2482_writeBytes((uint8_t[]){DS2482_COMMAND_SINGLEBIT, (data ? 0x80 : 0x00)}, 2);
}

// As wireWriteBit
uint8_t wireReadBit()
{
	wireWriteBit(1, false);
	uint8_t status = DS2482_waitOnBusy(true);
	return status & DS2482_STATUS_SBR ? 1 : 0;
}

// 1-Wire skip
void wireSkip()
{
	wireWriteByte(WIRE_COMMAND_SKIP, false);
}

void wireSelect(const uint8_t *rom)
{
	wireWriteByte(WIRE_COMMAND_SELECT, false);
	for (int i=0;i<8;i++)
		wireWriteByte(rom[i], false);
}

//  1-Wire reset search algorithm
void wireResetSearch()
{
	BC->searchLastDiscrepancy = 0;
	BC->searchExhausted = 0;

	for (int i = 0; i < 8; i++)
	{
		BC->searchAddress[i] = 0;
	}
}

uint8_t wireSearch(uint8_t *newAddr)
{
    uint8_t i;
    uint8_t direction;
    uint8_t last_zero=0;

    if (BC->searchExhausted)
        return 0;

    if (!wireReset())
        return 0;

    DS2482_waitOnBusy(true);
    wireWriteByte(0xf0, false);

    for(i=1;i<65;i++)
    {
        int romByte = (i-1)>>3;
        int romBit = 1<<((i-1)&7);

        if (i < BC->searchLastDiscrepancy)
            direction = BC->searchAddress[romByte] & romBit;
        else
            direction = i == BC->searchLastDiscrepancy;

        DS2482_waitOnBusy(false);
        DS2482_writeBytes((uint8_t[]){DS2482_COMMAND_TRIPLET, (direction ? 0x80 : 0x00)}, 2);

        uint8_t status = DS2482_waitOnBusy(false);

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

    BC->searchLastDiscrepancy = last_zero;

    if (last_zero == 0)
        BC->searchExhausted = 1;

    for (i=0;i<8;i++)
        newAddr[i] = BC->searchAddress[i];

    return 1;
}