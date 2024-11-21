#pragma once
#include "wx/wx.h"
#include "stdint.h"
#include "Channel.h"

/*Data structure used to store information used for decoding operation*/
typedef struct channelIndex {
	uint16_t channel0ToggleIndex;
	uint16_t channel1ToggleIndex;
	uint16_t channel2ToggleIndex;
	uint16_t channel3ToggleIndex;
	uint16_t channel4ToggleIndex;
	uint16_t channel5ToggleIndex;
	uint16_t channel6ToggleIndex;
	uint16_t channel7ToggleIndex;
} channelToggleIndex;


/*Functions used for decoding*/
// argument order wxDC pointer, the pointer to the samples, current zoom factor, y cord to draw, chanel width, Sampleindex, panelwidth
void DecodeI2C(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index, int length, int Samplelimit, Channel* channelptr);
void DecodeSPI(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index, int length, int Samplelimit, Channel* channelptr);
void DecodeUART(wxDC*, uint8_t*, uint8_t, uint16_t, uint16_t, uint16_t);
void DecodeParallel(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index, int length, bool linedecode, int Samplelimit);

void drawHexagon(wxDC*, int, int, int, int, int, uint8_t);
void drawTrailingPolygon(wxDC*, int, int, int, int, int, uint8_t);
wxString decodeHEXValue(uint8_t);
wxString decodeBinaryValue(uint8_t);