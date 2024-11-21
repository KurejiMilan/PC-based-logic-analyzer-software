#pragma once
#include<stdint.h>

#define I2C 1
#define SPI 2
#define UART 3
#define PARALLEL 4

class Channel {
public:
	uint8_t active8ChannelMask = 0xff;										// initially all channels are active, used as bitfield data type to indicate which channel is enabled
	uint8_t totalActiveChannel = 8;											// total no of active channel maximum can be 8, this is only used to mask on softwere and not on hardware for now
	uint8_t decodedChannel = 0;												// total no of channel dedicaded for decode function
	uint8_t triggerMask = 0x00;												// this is used to set trigger mask in hardware
	uint8_t triggerWord = 0x00;												// this is used to set either the rising or faling edge for edge trigger or setting trigger word in word trigger
	uint8_t triggerType = 0x00;												// 0 = none, 1 = edge trigger, 2 = word trigger
	unsigned int scrolledSample = 0;
	bool decodeLineState = true;												// this bool is used to decode state i.e. 1 or 0 of a line
	uint8_t sampleRateIndex = 2;
	uint8_t sampleSizeIndex = 3;

	uint16_t blockTransferSize = 32;

	uint8_t SelectedProtocolDecoder = 0;									// used to select one of the protocols to be decoded, 0 means no decoding

	float timeScale = 640.00;

	// for I2C 
	uint8_t SDAchannel = 0, SCLchannel = 0;
	// for SPI
	uint8_t SPICSchannel = 0, SPISCKchannel = 0, SPIMOSIchannel =0, SPIMISOchannel = 0;
};