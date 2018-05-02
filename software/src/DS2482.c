#include "DS2482.h"
#include "bricklib/utility/util_definitions.h"

#define I2C_EEPROM_ADDRESS_HIGH 84

#define I2C_ADDRESS_HIGH 25 // 0b0011001
#define I2C_ADDRESS_LOW 24 // 0b0011000

uint8_t mTimeout;

uint8_t DS2482_getAddress(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
	}
}

void DS2482_writeByte(uint8_t data)
{
    DS2482_writeBytes(&data, 1);
}

void DS2482_writeBytes(uint8_t* data, const uint8_t length)
{
    if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
        BA->bricklet_select(BS->port - 'a');
        BA->TWID_Write(BA->twid, DS2482_getAddress(), 0, 0, data, length, NULL);

        BA->bricklet_deselect(BS->port - 'a');
        BA->mutex_give(*BA->mutex_twi_bricklet);
    }
}

uint8_t DS2482_readByte()
{
    if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
        uint8_t value = 0;

        BA->bricklet_select(BS->port - 'a');
        BA->TWID_Read(BA->twid, DS2482_getAddress(), 0, 0, &value, 1, NULL);

        BA->bricklet_deselect(BS->port - 'a');
        BA->mutex_give(*BA->mutex_twi_bricklet);

        return value;
    }

    return 0;
}

// Performs a global reset of device state machine logic. Terminates any ongoing 1-Wire communication.
void DS2482_deviceReset()
{
    BC->mError = 0;
	DS2482_writeByte(DS2482_COMMAND_RESET);
}

// Sets the read pointer to the specified register. Overwrites the read pointer position of any 1-Wire communication command in progress.
void DS2482_setReadPointer(uint8_t readPointer)
{
	DS2482_writeBytes((uint8_t[]){DS2482_COMMAND_SRP, readPointer}, 2);
}

uint8_t DS2482_readStatus(bool setPtr)
{
    if (setPtr)
        DS2482_setReadPointer(DS2482_POINTER_STATUS);

    return DS2482_readByte();
}

// Read the data register
uint8_t DS2482_readData()
{
	DS2482_setReadPointer(DS2482_POINTER_DATA);
	return DS2482_readByte();
}

// Read the config register
uint8_t DS2482_readConfig()
{
	DS2482_setReadPointer(DS2482_POINTER_CONFIG);
	return DS2482_readByte();
}

void DS2482_setStrongPullup()
{
	DS2482_writeConfig(DS2482_readConfig() | DS2482_CONFIG_SPU);
}

void DS2482_clearStrongPullup()
{
	DS2482_writeConfig(DS2482_readConfig() & !DS2482_CONFIG_SPU);
}

uint8_t DS2482_waitOnBusy(bool setReadPtr)
{
    uint8_t status;
    int loopCount = 1000;
    while((status = DS2482_readStatus(setReadPtr)) & DS2482_STATUS_BUSY)
    {
        if (--loopCount <= 0)
        {
            mTimeout = 1;
            break;
        }
        SLEEP_US(20);
    }
    return status;
}

// Write to the config register
void DS2482_writeConfig(uint8_t config)
{
	DS2482_waitOnBusy(true);

	// Write the 4 bits and the complement 4 bits
	DS2482_writeBytes((uint8_t[]){DS2482_COMMAND_WRITECONFIG, (config | (~config)<<4)}, 2);

	// This should return the config bits without the complement
	if (DS2482_readByte() != config)
		BC->mError = DS2482_ERROR_CONFIG;
}