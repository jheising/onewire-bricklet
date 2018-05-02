#ifndef DS2482_H
#define DS2482_H

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

#define DS2482_ERROR_TIMEOUT		(1<<0)
#define DS2482_ERROR_SHORT			(1<<1)
#define DS2482_ERROR_CONFIG			(1<<2)

uint8_t DS2482_getAddress(void);
void DS2482_writeBytes(uint8_t* data, const uint8_t length);
void DS2482_writeByte(uint8_t data);
uint8_t DS2482_readByte();
void DS2482_deviceReset();
void DS2482_setReadPointer(uint8_t readPointer);
uint8_t DS2482_readStatus(bool setPtr);
uint8_t DS2482_readData();
uint8_t DS2482_readConfig();
void DS2482_setStrongPullup();
void DS2482_clearStrongPullup();
uint8_t DS2482_waitOnBusy(bool setReadPtr);
void DS2482_writeConfig(uint8_t config);

#endif

