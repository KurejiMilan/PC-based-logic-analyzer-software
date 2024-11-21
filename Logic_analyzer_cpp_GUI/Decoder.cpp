#include "Decoder.h"

/*
	Functions used to decode and draw the decoded data
	wxDC dc is used to draw to the wavePanel using the dc panel
	samplePtr is used to point to the sample, the sample values are acessed using this pointer
	zoom_factor is used to draw sample for certain number of pixel here it is used to show how many pixel does the decoded value is drawn
*/
// need to add channel width , and wxPoint
/*
	prevStableSample store sampled data that has not been changed 
	currentSample stores the sample pointed by the sample index, it' value is continiously updated to new sample data pointed by the
	sample index

	xor operation performed between these to find change in state of any channel
	prevStableSampleIndex is the index of the last stable sample, currentSampleIndex is the index of the sample that corresponds to change is state

*/
//	function used to decode I2C protocol
void DecodeI2C(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index, int length, int Samplelimit, Channel* channelptr) {
	// this algorithm is implemented based on the asumtion that the start condition has not been sent yet
	// and it is sent somewhere in the middle or else it won't work

	uint8_t prevSample = samplePtr[index];
	uint16_t lastDrawnIndex = index;
	uint16_t currentindex = index + 1;
	uint8_t currentSample = samplePtr[currentindex];

	// 0 specifies no I2C packet to decode
	// 1 specifies that start condition is identified and look for the address and the read or write bit
	// 2 specifies that  address has been identided now decode remaining as it simple 1 byte data followed by ack or nack
	// initially set to 0 only works for this condition
	uint8_t i2cPacketIndex = 0;
	
	uint8_t sclMask = 1 << channelptr->SCLchannel;					// used to mask out the scl channel
	uint8_t sdaMask = 1 << channelptr->SDAchannel;					// used to mask out the sda channel
	bool sdaLow = false, sclLow = false;

	uint8_t sclCounter = 0;
	uint8_t decodedSDA = 0x00;

	int fontSize = 0, polygonGap = 0;
	int clockSamplePoint = 0, referenceClockPoint = 0;
	bool startClockSamplePoint = false, fallingEdge1 = false;
	wxString decodedText;

	do {
		if (i2cPacketIndex == 0) {
			if ((!sdaLow) && (!sclLow)) {
				// to detect the sda low going low
				if ((currentSample & sdaMask) ^ (prevSample & sdaMask)) {
					// falling edge on sda line detected
					if (prevSample & sdaMask) {
						sdaLow = true;
						dc->DrawLine((lastDrawnIndex-index)*zoom_factor, yCord, (currentindex-index)*zoom_factor, yCord);
						lastDrawnIndex = currentindex;
					}
				}
			}

			if ((sdaLow) && (!sclLow)) {
				// to detect the sda low going low
				if ((currentSample & sclMask) ^ (prevSample & sclMask)) {
					// falling edge on sda line detected
					if (prevSample & sclMask) {
						sclLow = true;

						int pixelGap = ((currentindex - index) - (lastDrawnIndex - index)) * zoom_factor;
						int x1 = (lastDrawnIndex - index) * zoom_factor;
						int x2 = (currentindex - index) * zoom_factor;
						int y1 = yCord - (width / 2) + 2;
						int y2 = yCord + (width / 2) - 2;

						if (pixelGap >= 80) {
							fontSize = 9;
							polygonGap = 8;
						}
						else if ((pixelGap >= 50) && (pixelGap < 80)) {
							fontSize = 8;
							polygonGap = 4;
						}
						else if ((pixelGap >= 20) && (pixelGap < 50)) {
							fontSize = 8;
							polygonGap = 2;
						}
						else if ((pixelGap >= 11) && (pixelGap < 20)) {
							fontSize = 6;
							polygonGap = 1;
						}
						else {
							fontSize = 0;
							polygonGap = 1;
						}
						drawHexagon(dc, x1, x2, y1, y2, yCord, polygonGap);
						if (fontSize != 0) {
							wxFont font(fontSize, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);
							wxSize textSize;
							textSize = dc->GetTextExtent(wxString::Format(wxT("S")));
							dc->DrawText(wxString::Format(wxT("S")), wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
						}
						lastDrawnIndex = currentindex;
						i2cPacketIndex = 1;
					}
				}
			}
			
		}
		else if (i2cPacketIndex == 1) {
			if ((prevSample & sclMask) ^ (currentSample & sclMask)) {
				if (currentSample & sclMask) {
					// rising edge clock line
					sclCounter++;

					if (sclCounter == 1) startClockSamplePoint = true;

					if (sclCounter <= 7) {
						decodedSDA = (decodedSDA << 1) | ((prevSample & sdaMask) >> channelptr->SDAchannel);
						if (sclCounter == 7) {
							decodedText.Printf(decodeHEXValue(decodedSDA));
						}
					}
					else if (sclCounter == 8) {
						startClockSamplePoint = false;
						clockSamplePoint++;
						clockSamplePoint = (int)clockSamplePoint / 7;
						// if 8th bit is 1 or HIGH it is READ operation
						// else for 0 or LOW it is WRITE operation
						decodedSDA = (prevSample & sdaMask) >> channelptr->SDAchannel;
						if (decodedSDA) decodedText.Printf(wxT("R"));
						else decodedText.Printf(wxT("W"));
					}
					else if (sclCounter == 9) {
						// LOW is ACK, HIGH is NACK
						decodedSDA = (prevSample & sdaMask) >> channelptr->SDAchannel;
						if (decodedSDA) decodedText.Printf(wxT("NACK"));
						else decodedText.Printf(wxT("ACK"));
						decodedSDA = 0x00;
					}
				}
				else {
					// falling edge clock line
					// 
					//if (sclCounter == 7);
					//else if (sclCounter == 8);
					if ((sclCounter == 7) | (sclCounter == 8) | (sclCounter == 9)) {
						int pixelGap = ((currentindex - index) - (lastDrawnIndex - index)) * zoom_factor;
						int x1 = (lastDrawnIndex - index) * zoom_factor;
						int x2 = (currentindex - index) * zoom_factor;
						int y1 = yCord - (width / 2) + 2;
						int y2 = yCord + (width / 2) - 2;

						if (pixelGap >= 80) {
							fontSize = 9;
							polygonGap = 8;
						}
						else if ((pixelGap >= 50) && (pixelGap < 80)) {
							fontSize = 8;
							polygonGap = 4;
						}
						else if ((pixelGap >= 20) && (pixelGap < 50)) {
							fontSize = 8;
							polygonGap = 2;
						}
						else if ((pixelGap >= 10) && (pixelGap < 20)) {
							if (sclCounter == 8) fontSize = 8;
							else if (sclCounter == 9) fontSize = 7;
							else fontSize = 5;
							polygonGap = 1;
						}
						else {
							fontSize = 0;
							polygonGap = 1;
						}
						drawHexagon(dc, x1, x2, y1, y2, yCord, polygonGap);
						if (fontSize != 0) {
							wxFont font(fontSize, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);
							wxSize textSize;
							textSize = dc->GetTextExtent(decodedText);
							dc->DrawText(decodedText, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
						}
						lastDrawnIndex = currentindex;
						
						if (sclCounter == 9) {
							sclCounter = 0;
							i2cPacketIndex = 2;
							decodedSDA = 0x00;
						}
					}
				}
			}
			if (startClockSamplePoint) clockSamplePoint++;
		}
		else if (i2cPacketIndex == 2) {
			// fisrt we need to detect if stop condition is sent , if not then perform decoding operation
			if ((prevSample & sclMask) ^ (currentSample & sclMask)) {
				if (currentSample & sclMask) {
					// rising clock edge
					sclCounter++;
					if (sclCounter == 1) {
						referenceClockPoint = currentindex;
					}
					if (sclCounter <= 8) {
						decodedSDA = (decodedSDA << 1) | ((currentSample | sdaMask) >> channelptr->SDAchannel);
						if (sclCounter == 8) {
							decodedText.Printf(decodeHEXValue(decodedSDA));
						}
					}
					else if (sclCounter == 9) {
						// LOW is ACK, HIGH is NACK
						decodedSDA = (prevSample & sdaMask) >> channelptr->SDAchannel;
						if (decodedSDA) decodedText.Printf(wxT("NACK"));
						else decodedText.Printf(wxT("ACK"));
						decodedSDA = 0x00;
					}
					
				}
				else {
					if (sclCounter == 1) fallingEdge1 = true;
					
					if ((sclCounter == 8) | (sclCounter == 9)) {
						int pixelGap = ((currentindex - index) - (lastDrawnIndex - index)) * zoom_factor;
						int x1 = (lastDrawnIndex - index) * zoom_factor;
						int x2 = (currentindex - index) * zoom_factor;
						int y1 = yCord - (width / 2) + 2;
						int y2 = yCord + (width / 2) - 2;

						if (pixelGap >= 80) {
							fontSize = 9;
							polygonGap = 8;
						}
						else if ((pixelGap >= 50) && (pixelGap < 80)) {
							fontSize = 8;
							polygonGap = 4;
						}
						else if ((pixelGap >= 20) && (pixelGap < 50)) {
							fontSize = 8;
							polygonGap = 2;
						}
						else if ((pixelGap >= 10) && (pixelGap < 20)) {
							if (sclCounter == 9) fontSize = 7;
							else fontSize = 5;
							polygonGap = 1;
						}
						else {
							fontSize = 0;
							polygonGap = 1;
						}
						drawHexagon(dc, x1, x2, y1, y2, yCord, polygonGap);
						if (fontSize != 0) {
							wxFont font(fontSize, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);
							wxSize textSize;
							textSize = dc->GetTextExtent(decodedText);
							dc->DrawText(decodedText, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
						}
						lastDrawnIndex = currentindex;
						if (sclCounter == 9) {
							sclCounter = 0;
							decodedSDA = 0x00;
							fallingEdge1 = false;
							referenceClockPoint = currentindex;					// not necessary just to be sure
						}
					}
				}
			}

			if (((currentindex - referenceClockPoint) > clockSamplePoint) && (!fallingEdge1)) {
				if (currentSample & sdaMask) {
					// the data line must be high as well
					if (sclCounter == 1) {
						// this indicates end of trnsmission
						sdaLow = false;
						sclLow = false;
						decodedSDA = 0;
						clockSamplePoint = 0;
						i2cPacketIndex = 0;
						sclCounter = 0;
						int pixelGap = ((currentindex - index) - (lastDrawnIndex - index)) * zoom_factor;
						int x1 = (lastDrawnIndex - index) * zoom_factor;
						int x2 = (currentindex - index) * zoom_factor;
						int y1 = yCord - (width / 2) + 2;
						int y2 = yCord + (width / 2) - 2;

						if (pixelGap >= 80) {
							fontSize = 9;
							polygonGap = 8;
						}
						else if ((pixelGap >= 50) && (pixelGap < 80)) {
							fontSize = 6;
							polygonGap = 4;
						}
						else if ((pixelGap >= 20) && (pixelGap < 50)) {
							fontSize = 5;
							polygonGap = 2;
						}
						else {
							fontSize = 0;
							polygonGap = 1;
						}
						drawHexagon(dc, x1, x2, y1, y2, yCord, polygonGap);
						if (fontSize != 0) {
							wxFont font(fontSize, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);
							wxSize textSize;
							textSize = dc->GetTextExtent(wxString::Format(wxT("P")));
							dc->DrawText(wxString::Format(wxT("P")), wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
						}
						
						lastDrawnIndex = currentindex;
					}
				}
			}
		}
		
		prevSample = currentSample;
		currentindex++;
		currentSample = samplePtr[currentindex];
	} while ((length > (currentindex - index) * zoom_factor) && (currentindex < Samplelimit - 1));

	// if i2cpackindex is other then 0 then scan for remaining bits in the line and display
	if ((i2cPacketIndex == 1) | (i2cPacketIndex == 2)) {
		// do something
		// for now just adding drawing trailling part, further decoding can be added
		int pixelGap = ((currentindex - index) - (lastDrawnIndex - index)) * zoom_factor;
		int x1 = (lastDrawnIndex - index) * zoom_factor;
		int x2 = (currentindex - index) * zoom_factor;
		int y1 = yCord - (width / 2) + 2;
		int y2 = yCord + (width / 2) - 2;

		if (pixelGap >= 80) {
			polygonGap = 8;
		}
		else if ((pixelGap >= 50) && (pixelGap < 80)) {
			polygonGap = 4;
		}
		else if ((pixelGap >= 20) && (pixelGap < 50)) {
			polygonGap = 2;
		}
		else if ((pixelGap >= 10) && (pixelGap < 20)) {
			polygonGap = 1;
		}
		else {
			polygonGap = 1;
		}
		drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, polygonGap);
	}
	else {
		dc->DrawLine((lastDrawnIndex - index)* zoom_factor, yCord, length, yCord);
	}
}

// function used to decode SPI protocol
// only implemented for mode 0 operation
void DecodeSPI(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index, int length, int Samplelimit, Channel* channelptr) {
	// index is not changed
	uint8_t prevsample = samplePtr[index];
	// current sample and previous sample are only used for detecting the cs enable
	int currentindex = index+1;

	uint8_t currentsample = samplePtr[currentindex];

	int startEnableIndex = 0, endEnableIndex =0;
	bool foundStartEnableIndex = false, foundEndEnableIndex = false;

	int scanningIndex = index;											// this is used to indicte the last sample point that was drawn 
	int totalSPIPacket = 0;												// this might be useful
	uint16_t sckCounter = 0;

	uint8_t csMask = 1 << channelptr->SPICSchannel;
	uint8_t sckMask = 1 << channelptr->SPISCKchannel;
	uint8_t mosiMask = 1 << channelptr->SPIMOSIchannel;
	uint8_t misoMask = 1 << channelptr->SPIMISOchannel;

	uint8_t  MOSIByte =0x00, MISOByte = 0x00;

	// perform scan and draw operation
	// first determine the start chip enable and end chip enable
	if (!(prevsample & csMask)) {
		// this indicates that the CS is already enabled at the start so also performa sacnning back operation	
		
		
		if (false) {
			// this part is same as the one in the else statement needs few modification
			do {

				// this indicates change of state
				if ((prevsample & csMask) ^ (currentsample & csMask)) {
					// this indicates that the previous sample was high and in this sample it does to low cs start
					if (prevsample & csMask) {
						startEnableIndex = currentindex;
						foundStartEnableIndex = true;
					}
					else {
						endEnableIndex = currentindex;
						foundEndEnableIndex = true;
					}
				}
				prevsample = currentsample;
				currentindex++;
				currentsample = samplePtr[currentindex];

				// if entire packet is found decode and draw it
				if (foundStartEnableIndex && foundEndEnableIndex) {
					foundStartEnableIndex = false;
					foundEndEnableIndex = false;

					// draw the idle lines to start of the enable low
					// draw for MOSI
					dc->DrawLine(scanningIndex, yCord, startEnableIndex, yCord);
					// draw for MOSI
					dc->DrawLine(scanningIndex, yCord + width, startEnableIndex, yCord + width);

					uint8_t tempReg0 = samplePtr[startEnableIndex];
					uint8_t tempReg1;

					int startOfpolygon = startEnableIndex;	// this is used for polygon starting point

					for (int i = startEnableIndex + 1; i <= endEnableIndex; i++) {
						tempReg1 = samplePtr[i];
						if ((tempReg0 & sckMask) ^ (tempReg1 & sckMask)) {
							if (tempReg0 & sckMask) {
								// do nothing since this indicates falling edge
							}
							else {
								sckCounter += 1;
								MOSIByte |= (MOSIByte << 1) | (tempReg1 & mosiMask);
								MISOByte |= (MISOByte << 1) | (tempReg1 & misoMask);
								if (sckCounter == 1) {
									// draw for MOSI
									dc->DrawLine(startOfpolygon, yCord, i, yCord);
									// draw for MOSI
									dc->DrawLine(startOfpolygon, yCord + width, i, yCord + width);
									startOfpolygon = i;
								}
							}
							// 8 BIT display the decoded hex value
							if (sckCounter == 8) {
								// then perform draw operaton
								// draw for 
								int pixelGap = ((i - index) - (startOfpolygon - index)) * zoom_factor;
								// one value of pixelgap , x1 and x2 are enough
								int x1 = (startOfpolygon - index) * zoom_factor;
								int x2 = (i - index) * zoom_factor;

								int y1 = yCord - (width / 2) + 2;
								int y2 = yCord + (width / 2) - 2;

								if (pixelGap >= 80) {
									wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
									dc->SetFont(font);
									wxSize textSize;

									drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
									wxString decodedMosiString = decodeHEXValue(MOSIByte);
									textSize = dc->GetTextExtent(decodedMosiString);
									dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

									drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 8);
									wxString decodedMisoString = decodeHEXValue(MISOByte);
									textSize = dc->GetTextExtent(decodedMisoString);
									dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
								}

								else if ((pixelGap >= 50) && (pixelGap < 80)) {
									wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
									dc->SetFont(font);

									wxSize textSize;
									drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
									wxString decodedMosiString = decodeHEXValue(MOSIByte);
									textSize = dc->GetTextExtent(decodedMosiString);
									dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

									drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 4);
									wxString decodedMisoString = decodeHEXValue(MISOByte);
									textSize = dc->GetTextExtent(decodedMisoString);
									dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
								}
								else if ((pixelGap >= 20) && (pixelGap < 50)) {
									wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
									dc->SetFont(font);
									wxSize textSize;

									drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
									wxString decodedMosiString = decodeHEXValue(MOSIByte);
									textSize = dc->GetTextExtent(decodedMosiString);
									dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

									drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 2);
									wxString decodedMisoString = decodeHEXValue(MISOByte);
									textSize = dc->GetTextExtent(decodedMisoString);
									dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
								}
								else {
									drawHexagon(dc, x1, x2, y1, y2, yCord, 1);
									drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 1);
								}

								MOSIByte = 0X00;
								MISOByte = 0X00;
								sckCounter = 0;
								startOfpolygon = i;
							}

						}
						tempReg0 = tempReg1;
					}
					scanningIndex = endEnableIndex;
				}

			} while ((length > (currentindex - index) * zoom_factor) && (currentindex < Samplelimit - 1));
			// this is used when cs enable is found but cs disable is not found 
			if (foundStartEnableIndex && (!foundEndEnableIndex)) {

				bool scannedRemaining = false;
				int tempIndex = startEnableIndex + 1;
				uint8_t tempPrevSample = samplePtr[startEnableIndex];
				uint8_t tempCurrentSample = samplePtr[tempIndex];
				uint8_t tempsckCounter = 0;

				// scan where the cs is disabled or the sample is completed
				while (!scannedRemaining) {

					if ((tempPrevSample & csMask) ^ (tempCurrentSample & csMask)) {
						if (tempPrevSample & csMask);									// do nothing
						else {
							endEnableIndex = tempIndex;
							scannedRemaining = true;
						}
					}

					if (tempIndex == Samplelimit - 1) {
						endEnableIndex = tempIndex;
						scannedRemaining = true;
					}

					if ((tempPrevSample & sckMask) ^ (tempCurrentSample & sckMask)) {
						if (tempPrevSample & sckMask);								// this means falling edge so do nothing
						else {
							tempsckCounter += 1;										// keeping count of how many bits has been tramitted
						}
					}

					tempPrevSample = tempCurrentSample;
					tempIndex++;
					tempCurrentSample = samplePtr[tempIndex];
				}

				// draw for MOSI
				dc->DrawLine(scanningIndex, yCord, startEnableIndex, yCord);
				// draw for MOSI
				dc->DrawLine(scanningIndex, yCord + width, startEnableIndex, yCord + width);

				uint8_t tempReg0 = samplePtr[startEnableIndex];
				uint8_t tempReg1;

				int startOfpolygon = startEnableIndex;

				for (int j = startEnableIndex + 1; j < endEnableIndex; j++) {
					tempReg1 = samplePtr[j];

					if ((tempReg0 & sckMask) ^ (tempReg1 & sckMask)) {
						if (tempReg0 & sckMask);
						else {
							sckCounter++;

							MOSIByte |= (MOSIByte << 1) | (tempReg1 & mosiMask);
							MISOByte |= (MISOByte << 1) | (tempReg1 & misoMask);
							// draw strignt line to the start of polygon
							if (sckCounter == 1) {
								// draw for MOSI
								dc->DrawLine(startOfpolygon, yCord, j, yCord);
								// draw for MOSI
								dc->DrawLine(startOfpolygon, yCord + width, j, yCord + width);
								startOfpolygon = j;
							}
						}

						if (sckCounter == 8) {
							// then perform draw operaton
							// draw for 
							int pixelGap = ((j - index) - (startOfpolygon - index)) * zoom_factor;
							// one value of pixelgap , x1 and x2 are enough
							int x1 = (startOfpolygon - index) * zoom_factor;
							int x2 = (j - index) * zoom_factor;

							int y1 = yCord - (width / 2) + 2;
							int y2 = yCord + (width / 2) - 2;

							if (pixelGap >= 80) {
								wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
								dc->SetFont(font);
								wxSize textSize;

								drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
								wxString decodedMosiString = decodeHEXValue(MOSIByte);
								textSize = dc->GetTextExtent(decodedMosiString);
								dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

								drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 8);
								wxString decodedMisoString = decodeHEXValue(MISOByte);
								textSize = dc->GetTextExtent(decodedMisoString);
								dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
							}

							else if ((pixelGap >= 50) && (pixelGap < 80)) {
								wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
								dc->SetFont(font);

								wxSize textSize;
								drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
								wxString decodedMosiString = decodeHEXValue(MOSIByte);
								textSize = dc->GetTextExtent(decodedMosiString);
								dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

								drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 4);
								wxString decodedMisoString = decodeHEXValue(MISOByte);
								textSize = dc->GetTextExtent(decodedMisoString);
								dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
							}
							else if ((pixelGap >= 20) && (pixelGap < 50)) {
								wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
								dc->SetFont(font);
								wxSize textSize;

								drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
								wxString decodedMosiString = decodeHEXValue(MOSIByte);
								textSize = dc->GetTextExtent(decodedMosiString);
								dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

								drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 2);
								wxString decodedMisoString = decodeHEXValue(MISOByte);
								textSize = dc->GetTextExtent(decodedMisoString);
								dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
							}
							else {
								drawHexagon(dc, x1, x2, y1, y2, yCord, 1);
								drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord, 1);
							}

							MOSIByte = 0X00;
							MISOByte = 0X00;
							sckCounter = 0;
							startOfpolygon = j;
						}
					}

					// if the sample is complete just draw trailling edge
					if ((length >= ((j - index) * zoom_factor)) && (sckCounter % 8 != 0)) {
						int pixelGap = ((j - index) - (startOfpolygon - index)) * zoom_factor;					// not used for now.
						int x1 = (startOfpolygon - index) * zoom_factor;
						int x2 = (j - index) * zoom_factor;
						int y1 = yCord - (width / 2) + 2;
						int y2 = yCord + (width / 2) - 2;
						// draw traling edge of MOSI
						drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, 8);
						// draw trailing edge of MISO
						drawTrailingPolygon(dc, x1, x2, y1 + width, y2 + width, yCord, 8);

						break;
					}
					tempReg0 = tempReg1;
				}
				sckCounter = 0;
			}
			else {
				//  draw stright line lines 
				// draw for MOSI
				dc->DrawLine(scanningIndex, yCord, (currentindex - index) * zoom_factor, yCord);
				// draw for MOSI
				dc->DrawLine(scanningIndex, yCord + width, (currentindex - index) * zoom_factor, yCord + width);
			}
		}
	}
	else {
		// this indicates that CS is not enabled perform scanning from the start
		// scan the cs line to get starting and ending point
		do {
			
			// this indicates change of state
			if ((prevsample & csMask) ^ (currentsample & csMask)) {
				// this indicates that the previous sample was high and in this sample it does to low cs start
				if (prevsample & csMask) {
					startEnableIndex = currentindex;
					foundStartEnableIndex = true;
				}
				else {
					endEnableIndex = currentindex;
					foundEndEnableIndex = true;
				}
			}
			// if entire packet is found decode and draw it
			if (foundStartEnableIndex && foundEndEnableIndex) {
				// draw the idle lines to start of the enable low
				// draw for MOSI
				dc->DrawLine((scanningIndex-index)* zoom_factor, yCord, (startEnableIndex-index)* zoom_factor, yCord);
				// draw for MOSI
				dc->DrawLine((scanningIndex-index)* zoom_factor, yCord + width, (startEnableIndex-index)* zoom_factor, yCord + width);

				uint8_t tempReg0 = samplePtr[startEnableIndex];
				uint8_t tempReg1;
				
				int startOfpolygon = startEnableIndex;	// this is used for polygon starting point

				for (int i = startEnableIndex+1; i <= endEnableIndex; i++) {
					tempReg1 = samplePtr[i];
					if ((tempReg0 & sckMask) ^ (tempReg1 & sckMask)) {
						if (tempReg0 & sckMask);
						else {
							sckCounter += 1;
							MOSIByte = (MOSIByte << 1) | ((tempReg1 & mosiMask) >> channelptr->SPIMOSIchannel);
							MISOByte = (MISOByte << 1) | ((tempReg1 & misoMask) >> channelptr->SPIMISOchannel);

							if (sckCounter == 1) {
								// draw for MOSI    
								dc->DrawLine((startOfpolygon-index)* zoom_factor, yCord, (i-index)* zoom_factor, yCord);
								// draw for MOSI
								dc->DrawLine((startOfpolygon-index)* zoom_factor, yCord + width, (i-index)* zoom_factor, yCord + width);
								startOfpolygon = i;
							}
						}
						// 8 BIT display the decoded hex value
						if (sckCounter == 8) {
							// then perform draw operaton
							// draw for 
							int pixelGap = ((i - index)- (startOfpolygon - index)) * zoom_factor;
							// one value of pixelgap , x1 and x2 are enough
							int x1 = (startOfpolygon - index) * zoom_factor;
							int x2 = (i - index) * zoom_factor;

							int y1 = yCord - (width / 2) + 2;
							int y2 = yCord + (width / 2) - 2;

							if (pixelGap >= 80) {
								wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
								dc->SetFont(font);
								wxSize textSize;

								drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
								wxString decodedMosiString = decodeHEXValue(MOSIByte);
								textSize = dc->GetTextExtent(decodedMosiString);
								dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
								
								drawHexagon(dc, x1, x2, y1+width, y2+width, yCord + width, 8);
								wxString decodedMisoString = decodeHEXValue(MISOByte);
								textSize = dc->GetTextExtent(decodedMisoString);
								dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1+width));
							}

							else if ((pixelGap >= 50) && (pixelGap < 80)) {
								wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
								dc->SetFont(font);

								wxSize textSize;
								drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
								wxString decodedMosiString = decodeHEXValue(MOSIByte);
								textSize = dc->GetTextExtent(decodedMosiString);
								dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

								drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord + width, 4);
								wxString decodedMisoString = decodeHEXValue(MISOByte);
								textSize = dc->GetTextExtent(decodedMisoString);
								dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
							}
							else if ((pixelGap >= 20) && (pixelGap < 50)) {
								wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
								dc->SetFont(font);
								wxSize textSize;

								drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
								wxString decodedMosiString = decodeHEXValue(MOSIByte);
								textSize = dc->GetTextExtent(decodedMosiString);
								dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

								drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord + width, 2);
								wxString decodedMisoString = decodeHEXValue(MISOByte);
								textSize = dc->GetTextExtent(decodedMisoString);
								dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
							}
							else {
								drawHexagon(dc, x1, x2, y1, y2, yCord, 1);
								drawHexagon(dc, x1, x2, y1+width, y2+width, yCord + width, 1);
							}

							MOSIByte = 0X00;
							MISOByte = 0X00;
							sckCounter = 0;
							startOfpolygon = i;
						}

					}
					tempReg0 = tempReg1;
				}
				// draw the last line up to the point where cs goes high
				// draw for MOSI    
				dc->DrawLine((startOfpolygon - index) * zoom_factor, yCord, (endEnableIndex - index) * zoom_factor, yCord);
				// draw for MOSI
				dc->DrawLine((startOfpolygon - index) * zoom_factor, yCord + width, (endEnableIndex - index) * zoom_factor, yCord + width);

				scanningIndex = endEnableIndex;
				foundStartEnableIndex = false;
				foundEndEnableIndex = false;
			}
			
			prevsample = currentsample;
			currentindex++;
			currentsample = samplePtr[currentindex];

		} while ((length > (currentindex - index) * zoom_factor) && (currentindex < Samplelimit - 1));
		
		// this is used when cs enable is found but cs disable is not found 
		if (foundStartEnableIndex && (!foundEndEnableIndex)) {
			
			bool scannedRemaining = false;
			uint8_t tempPrevSample = samplePtr[startEnableIndex];
			int tempIndex = startEnableIndex+1;
			uint8_t tempCurrentSample = samplePtr[tempIndex];
			uint8_t tempsckCounter = 0;

			// scan where the cs is disabled or the sample is completed
			while (!scannedRemaining) {
				
				if ((tempPrevSample & csMask) ^ (tempCurrentSample & csMask)) {
					if (tempPrevSample & csMask);									// do nothing
					else {
						endEnableIndex = tempIndex;
						scannedRemaining = true;
					}
				}

				if (tempIndex == Samplelimit - 1) {
					endEnableIndex = tempIndex;
					scannedRemaining = true;
				}

				if ((tempPrevSample & sckMask) ^ (tempCurrentSample & sckMask)) {
					if (tempPrevSample & sckMask);								// this means falling edge so do nothing
					else {
						tempsckCounter += 1;										// keeping count of how many bits has been tramitted
					}
				}

				tempPrevSample = tempCurrentSample;
				tempIndex++;
				tempCurrentSample = samplePtr[tempIndex];
			}
			
			// draw for MOSI
			dc->DrawLine((scanningIndex-index)*zoom_factor, yCord, (startEnableIndex-index)*zoom_factor, yCord);
			// draw for MOSI
			dc->DrawLine((scanningIndex-index)*zoom_factor, yCord + width, (startEnableIndex-index)*zoom_factor, yCord + width);

			uint8_t tempReg0 = samplePtr[startEnableIndex];
			uint8_t tempReg1;

			int startOfpolygon = startEnableIndex;
			
			for (int j = startEnableIndex+1; j <= endEnableIndex; j++) {
				tempReg1 = samplePtr[j];

				if ((tempReg0 & sckMask) ^ (tempReg1 & sckMask)) {
					if (tempReg0 & sckMask);
					else {
						sckCounter++;
						MOSIByte = (MOSIByte << 1) | ((tempReg1 & mosiMask) >> channelptr->SPIMOSIchannel);
						MISOByte = (MISOByte << 1) | ((tempReg1 & misoMask) >> channelptr->SPIMISOchannel);
						// draw strignt line to the start of polygon
						if (sckCounter == 1) {
							// draw for MOSI
							dc->DrawLine((startOfpolygon-index)*zoom_factor, yCord, (j-index)*zoom_factor, yCord);
							// draw for MISO
							dc->DrawLine((startOfpolygon-index)*zoom_factor, yCord + width, (j-index)*zoom_factor, yCord + width);
							startOfpolygon = j;
						}
					}

					if (sckCounter == 8) {
						// then perform draw operaton
						// draw for 
						int pixelGap = ((j - index) - (startOfpolygon - index)) * zoom_factor;
						// one value of pixelgap , x1 and x2 are enough
						int x1 = (startOfpolygon - index) * zoom_factor;
						int x2 = (j - index) * zoom_factor;

						int y1 = yCord - (width / 2) + 2;
						int y2 = yCord + (width / 2) - 2;

						if (pixelGap >= 80) {
							wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);
							wxSize textSize;

							drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
							wxString decodedMosiString = decodeHEXValue(MOSIByte);
							textSize = dc->GetTextExtent(decodedMosiString);
							dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

							drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord + width, 8);
							wxString decodedMisoString = decodeHEXValue(MISOByte);
							textSize = dc->GetTextExtent(decodedMisoString);
							dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
						}
						else if ((pixelGap >= 50) && (pixelGap < 80)) {
							wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);

							wxSize textSize;
							drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
							wxString decodedMosiString = decodeHEXValue(MOSIByte);
							textSize = dc->GetTextExtent(decodedMosiString);
							dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

							drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord + width, 4);
							wxString decodedMisoString = decodeHEXValue(MISOByte);
							textSize = dc->GetTextExtent(decodedMisoString);
							dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
						}
						else if ((pixelGap >= 20) && (pixelGap < 50)) {
							wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
							dc->SetFont(font);
							wxSize textSize;

							drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
							wxString decodedMosiString = decodeHEXValue(MOSIByte);
							textSize = dc->GetTextExtent(decodedMosiString);
							dc->DrawText(decodedMosiString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));

							drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord + width, 2);
							wxString decodedMisoString = decodeHEXValue(MISOByte);
							textSize = dc->GetTextExtent(decodedMisoString);
							dc->DrawText(decodedMisoString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1 + width));
						}
						else {
							drawHexagon(dc, x1, x2, y1, y2, yCord, 1);
							drawHexagon(dc, x1, x2, y1 + width, y2 + width, yCord + width, 1);
						}

						MOSIByte = 0X00;
						MISOByte = 0X00;
						sckCounter = 0;
						startOfpolygon = j;
					}
				}

				// if the sample is complete just draw trailling edge
				if ((length <= ((j - index) * zoom_factor)) && ((sckCounter%8) != 0)) {
					
					//int pixelGap = ((j - index) - (startOfpolygon - index)) * zoom_factor;					// not used for now.
					int x1 = (startOfpolygon - index) * zoom_factor;
					int x2 = (j - index) * zoom_factor;
					int y1 = yCord - (width / 2) + 2;
					int y2 = yCord + (width / 2) - 2;
					// draw traling edge of MOSI
					drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, 8);
					// draw trailing edge of MISO
					drawTrailingPolygon(dc, x1, x2, y1+width, y2+width, yCord + width, 8);

					break;
				}
				tempReg0 = tempReg1;
			}
			sckCounter = 0;
		}
		else {
			//  draw stright line lines 
			// draw for MOSI
			dc->DrawLine((scanningIndex - index)* zoom_factor, yCord, (currentindex - index)* zoom_factor, yCord);
			// draw for MOSI
			dc->DrawLine((scanningIndex - index)* zoom_factor, yCord + width, (currentindex - index)* zoom_factor, yCord + width);
		}
	}
}

//	function used to decode UART protocol
void DecodeUART(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index) {
	uint8_t prevStableSample = samplePtr[index];
	uint8_t currentSample = samplePtr[index + 1];
	uint16_t prevStableSampleIndex = index, currentSampleIndex = index + 1;

	/*channelToggleIndex* prevChannelToggleIndex = (channelToggleIndex*)malloc(sizeof(channelToggleIndex));

	prevChannelToggleIndex->channel0ToggleIndex = index;
	prevChannelToggleIndex->channel1ToggleIndex = index;
	prevChannelToggleIndex->channel2ToggleIndex = index;
	prevChannelToggleIndex->channel3ToggleIndex = index;
	prevChannelToggleIndex->channel4ToggleIndex = index;
	prevChannelToggleIndex->channel5ToggleIndex = index;
	prevChannelToggleIndex->channel6ToggleIndex = index;
	prevChannelToggleIndex->channel7ToggleIndex = index;
	index++;
	
	free(prevChannelToggleIndex);*/
}

//	function used to decode Parallel
// this function performs both parallel decoding and line state decoded display function
void DecodeParallel(wxDC* dc, uint8_t* samplePtr, uint8_t zoom_factor, uint16_t yCord, uint16_t width, uint16_t index, int length, bool linedecode, int Samplelimit) {
	uint16_t startIndex = index;
	uint8_t prevStableSample = samplePtr[index];
	uint8_t currentSample = samplePtr[index+1];
	uint16_t prevStableSampleIndex = index;

	channelToggleIndex* prevChannelToggleIndex = (channelToggleIndex*)malloc(sizeof(channelToggleIndex));

	prevChannelToggleIndex->channel0ToggleIndex = index;
	prevChannelToggleIndex->channel1ToggleIndex = index;
	prevChannelToggleIndex->channel2ToggleIndex = index;
	prevChannelToggleIndex->channel3ToggleIndex = index;
	prevChannelToggleIndex->channel4ToggleIndex = index;
	prevChannelToggleIndex->channel5ToggleIndex = index;
	prevChannelToggleIndex->channel6ToggleIndex = index;
	prevChannelToggleIndex->channel7ToggleIndex = index;
	index++;

	do {
		if (prevStableSample ^ currentSample) {
			int pixelGap = ((index - startIndex) - (prevStableSampleIndex - startIndex)) * zoom_factor;
			int x1 = (prevStableSampleIndex - startIndex) * zoom_factor;
			int x2 = (index - startIndex) * zoom_factor;
			int y1 = yCord - (width / 2) + 2;
			int y2 = yCord + (width / 2) - 2;
			
			// This section handle to draw line state i.e. 1 or 0
			if (linedecode) {
				// channel 1 state
				if ((prevStableSample ^ currentSample) & 0b00000001) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel0ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel0ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15;
					int ys2 = 15 + width * 1;
					uint8_t state = (prevStableSample & 0b00000001) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1)/2 - stateTextSize.x / 2 + xs1, (ys2 - ys1)/2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel0ToggleIndex = index;
				}
				// channel 2 state
				if ((prevStableSample ^ currentSample) & 0b00000010) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel1ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel1ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15+width * 1;
					int ys2 = 15 + width * 2;
					uint8_t state = (prevStableSample & 0b00000010) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel1ToggleIndex = index;
				}
				// channel 3
				if ((prevStableSample ^ currentSample) & 0b00000100) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel2ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel2ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15 + width * 2;
					int ys2 = 15 + width * 3;
					uint8_t state = (prevStableSample & 0b00000100) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel2ToggleIndex = index;
				}
				// channel 4
				if ((prevStableSample ^ currentSample) & 0b00001000) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel3ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel3ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15 + width * 3;
					int ys2 = 15 + width * 4;
					uint8_t state = (prevStableSample & 0b00001000) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel3ToggleIndex = index;
				}
				// channel 5
				if ((prevStableSample ^ currentSample) & 0b00010000) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel4ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel4ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15 + width * 4;
					int ys2 = 15 + width * 5;
					uint8_t state = (prevStableSample & 0b00010000) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel4ToggleIndex = index;
				}
				// channel 6
				if ((prevStableSample ^ currentSample) & 0b00100000) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel5ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel5ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15 + width * 5;
					int ys2 = 15 + width * 6;
					uint8_t state = (prevStableSample & 0b00100000) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel5ToggleIndex = index;
				}
				// channel 7
				if ((prevStableSample ^ currentSample) & 0b01000000) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel6ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel6ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15 + width * 6;
					int ys2 = 15 + width * 7;
					uint8_t state = (prevStableSample & 0b01000000) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel6ToggleIndex = index;
				}
				// channel 8
				if ((prevStableSample ^ currentSample) & 0b10000000) {
					int stateTimeGap = (index - prevChannelToggleIndex->channel7ToggleIndex) * zoom_factor;
					int xs1 = (prevChannelToggleIndex->channel7ToggleIndex - startIndex) * zoom_factor;
					int xs2 = (index - startIndex) * zoom_factor;
					int ys1 = 15 + width * 7;
					int ys2 = 15 + width * 8;
					uint8_t state = (prevStableSample & 0b10000000) ? 1 : 0;
					if (stateTimeGap >= 80) {
						wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
						wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
						wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
						dc->SetFont(stateFont);
						wxString stateText;
						stateText.Printf(wxT("%d"), state);
						wxSize stateTextSize = dc->GetTextExtent(stateText);
						dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
					}
					prevChannelToggleIndex->channel7ToggleIndex = index;
				}
			}

			// This section handles drawing decoded signal
			if (pixelGap >= 80) {
				drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
				wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxString decodedString = decodeBinaryValue(prevStableSample);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else if ((pixelGap >= 50) && (pixelGap < 80)) {
				drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
				wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxString decodedString = decodeBinaryValue(prevStableSample);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else if ((pixelGap >= 20) && (pixelGap < 50)) {
				drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
				wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxString decodedString = decodeBinaryValue(prevStableSample);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else drawHexagon(dc, x1, x2, y1, y2, yCord,1);
			
			prevStableSample = currentSample;
			prevStableSampleIndex = index;
		}
		index++;
		currentSample = samplePtr[index];
	} while ((length > (index - startIndex)*zoom_factor)&&(index<Samplelimit-1));
	

	// This section handle to draw line state i.e. 1 or 0
	if (linedecode) {
		// channel 1 state
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel0ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel0ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15;
			int ys2 = 15 + width * 1;
			uint8_t state = (prevStableSample & 0b00000001) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel0ToggleIndex = index;
		}
		// channel 2 state
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel1ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel1ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 1;
			int ys2 = 15 + width * 2;
			uint8_t state = (prevStableSample & 0b00000010) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel1ToggleIndex = index;
		}
		// channel 3
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel2ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel2ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 2;
			int ys2 = 15 + width * 3;
			uint8_t state = (prevStableSample & 0b00000100) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel2ToggleIndex = index;
		}
		// channel 4
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel3ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel3ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 3;
			int ys2 = 15 + width * 4;
			uint8_t state = (prevStableSample & 0b00001000) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel3ToggleIndex = index;
		}
		// channel 5
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel4ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel4ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 4;
			int ys2 = 15 + width * 5;
			uint8_t state = (prevStableSample & 0b00010000) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel4ToggleIndex = index;
		}
		// channel 6
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel5ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel5ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 5;
			int ys2 = 15 + width * 6;
			uint8_t state = (prevStableSample & 0b00100000) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel5ToggleIndex = index;
		}
		// channel 7
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel6ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel6ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 6;
			int ys2 = 15 + width * 7;
			uint8_t state = (prevStableSample & 0b01000000) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel6ToggleIndex = index;
		}
		// channel 8
		if (true) {
			int stateTimeGap = (index - prevChannelToggleIndex->channel7ToggleIndex) * zoom_factor;
			int xs1 = (prevChannelToggleIndex->channel7ToggleIndex - startIndex) * zoom_factor;
			int xs2 = (index - startIndex) * zoom_factor;
			int ys1 = 15 + width * 7;
			int ys2 = 15 + width * 8;
			uint8_t state = (prevStableSample & 0b10000000) ? 1 : 0;
			if (stateTimeGap >= 80) {
				wxFont stateFont(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 80) && (stateTimeGap >= 50)) {
				wxFont stateFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			else if ((stateTimeGap < 50) && (stateTimeGap >= 10)) {
				wxFont stateFont(7, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(stateFont);
				wxString stateText;
				stateText.Printf(wxT("%d"), state);
				wxSize stateTextSize = dc->GetTextExtent(stateText);
				dc->DrawText(stateText, wxPoint((xs2 - xs1) / 2 - stateTextSize.x / 2 + xs1, (ys2 - ys1) / 2 - stateTextSize.y / 2 + ys1));
			}
			prevChannelToggleIndex->channel7ToggleIndex = index;
		}
	}
	// The last sample pointed by the index is not draw it requires extra code to draw that part 
	// if the index is at the last memory index then polygon needs to be drawn
	if (index == Samplelimit-1) {
		int pixelGap = ((index - startIndex) - (prevStableSampleIndex - startIndex)) * zoom_factor;
		int x1 = (prevStableSampleIndex - startIndex) * zoom_factor;
		int x2 = (index - startIndex) * zoom_factor;
		int y1 = yCord - (width / 2) + 2;
		int y2 = yCord + (width / 2) - 2;
		wxString decodedString = decodeBinaryValue(prevStableSample);
		if (pixelGap >= 80) {
			drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
			wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
			dc->SetFont(font);
			wxSize textSize = dc->GetTextExtent(decodedString);
			dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
		}
		else if ((pixelGap >= 50) && (pixelGap < 80)) {
			drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
			wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
			dc->SetFont(font);
			wxSize textSize = dc->GetTextExtent(decodedString);
			dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
		}
		else if ((pixelGap >= 20) && (pixelGap < 50)) {
			drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
			wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
			dc->SetFont(font);
			wxSize textSize = dc->GetTextExtent(decodedString);
			dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
		}
		else drawHexagon(dc, x1, x2, y1, y2, yCord, 1);
	}

	// Either draw trailling polygon or hexagon for others
	else {
		// for drawing polygon
		if (currentSample ^ samplePtr[index + 1]) {
			int pixelGap = ((index - startIndex) - (prevStableSampleIndex - startIndex)) * zoom_factor;
			int x1 = (prevStableSampleIndex - startIndex) * zoom_factor;
			int x2 = (index - startIndex) * zoom_factor;
			int y1 = yCord - (width / 2) + 2;
			int y2 = yCord + (width / 2) - 2;
			wxString decodedString = decodeBinaryValue(prevStableSample);
			if (pixelGap >= 80) {
				drawHexagon(dc, x1, x2, y1, y2, yCord, 8);
				wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else if ((pixelGap >= 50) && (pixelGap < 80)) {
				drawHexagon(dc, x1, x2, y1, y2, yCord, 4);
				wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else if ((pixelGap >= 20) && (pixelGap < 50)) {
				drawHexagon(dc, x1, x2, y1, y2, yCord, 2);
				wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else drawHexagon(dc, x1, x2, y1, y2, yCord, 1);
		}
		// for drawing trailing polygon
		else {
			int pixelGap = length - (prevStableSampleIndex - startIndex) * zoom_factor;
			int x1 = (prevStableSampleIndex - startIndex) * zoom_factor;
			int x2 = (index - startIndex) * zoom_factor;
			int y1 = yCord - (width / 2) + 2;
			int y2 = yCord + (width / 2) - 2;
			wxString decodedString = decodeBinaryValue(prevStableSample);
			if (pixelGap >= 80) {
				drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, 8);
				wxFont font(9, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint((x2 - x1) / 2 - textSize.x / 2 + x1, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else if ((pixelGap >= 50) && (pixelGap < 80)) {
				drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, 4);
				wxFont font(6, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint(x1+10, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else if ((pixelGap >= 20) && (pixelGap < 50)) {
				drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, 2);
				wxFont font(5, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
				dc->SetFont(font);
				wxSize textSize = dc->GetTextExtent(decodedString);
				dc->DrawText(decodedString, wxPoint(x1+10, (y2 - y1) / 2 - textSize.y / 2 + y1));
			}
			else drawTrailingPolygon(dc, x1, x2, y1, y2, yCord, 1);
		}
	}

	free(prevChannelToggleIndex);
}

void drawHexagon(wxDC* dc, int x1, int x2, int y1, int y2, int midpoint, uint8_t x)
{
	/*
	  1	  ___________  2
		 /			 \
	  0	/             \  3
		\             /
	  5  \___________/  4

	*/
	wxPoint poly[6];
	// point 0
	poly[0].x = x1;
	poly[0].y = midpoint;
	// point 1
	poly[1].x = x1+x;
	poly[1].y = y1;
	// point 2
	poly[2].x = x2-x;
	poly[2].y = y1;
	// point 3
	poly[3].x = x2;
	poly[3].y = midpoint;
	// point 4
	poly[4].x = x2-x;
	poly[4].y = y2;
	// point 5
	poly[5].x = x1+x;
	poly[5].y = y2;

	dc->DrawPolygon(6, poly);
	//dc->DrawText(wxT("Hello"), 420, 395);
}

void drawTrailingPolygon(wxDC* dc, int x1, int x2, int y1, int y2, int midpoint, uint8_t x)
{
	/*
  1	  ___________  0
	 /			 
  2	/               
	\             
  3  \___________ 4

*/
	wxPoint poly[6];
	// point 0
	poly[0].x = x2;
	poly[0].y = y1;
	// point 1
	poly[1].x = x1 + x;
	poly[1].y = y1;
	// point 2
	poly[2].x = x1;
	poly[2].y = midpoint;
	// point 3
	poly[3].x = x1 + x;
	poly[3].y = y2;
	// point 4
	poly[4].x = x2;
	poly[4].y = y2;

	dc->DrawPolygon(5, poly);
}


// this function returns the HEX string of the 8 bit data 
wxString decodeHEXValue(uint8_t data) {
	wxString lsbHEX;
	wxString msbHEX;
	// for lower 4 bits
	if ((data & 0x0F) == 0x00) {
		lsbHEX.Printf(wxT("0"));
	}
	else if ((data & 0x0F) == 0x01) {
		lsbHEX.Printf(wxT("1"));
	}
	else if ((data & 0x0F) == 0x02) {
		lsbHEX.Printf(wxT("2"));
	}
	else if ((data & 0x0F) == 0x03) {
		lsbHEX.Printf(wxT("3"));
	}
	else if ((data & 0x0F) == 0x04) {
		lsbHEX.Printf(wxT("4"));
	}
	else if ((data & 0x0F) == 0x05) {
		lsbHEX.Printf(wxT("5"));
	}
	else if ((data & 0x0F) == 0x06) {
		lsbHEX.Printf(wxT("6"));
	}
	else if ((data & 0x0F) == 0x07) {
		lsbHEX.Printf(wxT("7"));
	}
	else if ((data & 0x0F) == 0x08) {
		lsbHEX.Printf(wxT("8"));
	}
	else if ((data & 0x0F) == 0x09) {
		lsbHEX.Printf(wxT("9"));
	}
	else if ((data & 0x0F) == 0x0A) {
		lsbHEX.Printf(wxT("A"));
	}
	else if ((data & 0x0F) == 0x0B) {
		lsbHEX.Printf(wxT("B"));
	}
	else if ((data & 0x0F) == 0x0C) {
		lsbHEX.Printf(wxT("C"));
	}
	else if ((data & 0x0F) == 0x0D) {
		lsbHEX.Printf(wxT("D"));
	}
	else if ((data & 0x0F) == 0x0E) {
		lsbHEX.Printf(wxT("E"));
	}
	else if ((data & 0x0F) == 0x0F) {
		lsbHEX.Printf(wxT("F"));
	}

	// for upper 4 bits
	if ((data & 0xF0) == 0x00) {
		msbHEX.Printf(wxT("0"));
	}
	else if ((data & 0xF0) == 0x10) {
		msbHEX.Printf(wxT("1"));
	}
	else if ((data & 0xF0) == 0x20) {
		msbHEX.Printf(wxT("2"));
	}
	else if ((data & 0xF0) == 0x30) {
		msbHEX.Printf(wxT("3"));
	}
	else if ((data & 0xF0) == 0x40) {
		msbHEX.Printf(wxT("4"));
	}
	else if ((data & 0xF0) == 0x50) {
		msbHEX.Printf(wxT("5"));
	}
	else if ((data & 0xF0) == 0x60) {
		msbHEX.Printf(wxT("6"));
	}
	else if ((data & 0xF0) == 0x70) {
		msbHEX.Printf(wxT("7"));
	}
	else if ((data & 0xF0) == 0x80) {
		msbHEX.Printf(wxT("8"));
	}
	else if ((data & 0xF0) == 0x90) {
		msbHEX.Printf(wxT("9"));
	}
	else if ((data & 0xF0) == 0xA0) {
		msbHEX.Printf(wxT("A"));
	}
	else if ((data & 0xF0) == 0xB0) {
		msbHEX.Printf(wxT("B"));
	}
	else if ((data & 0xF0) == 0xC0) {
		msbHEX.Printf(wxT("C"));
	}
	else if ((data & 0xF0) == 0xD0) {
		msbHEX.Printf(wxT("D"));
	}
	else if ((data & 0xF0) == 0xE0) {
		msbHEX.Printf(wxT("E"));
	}
	else if ((data & 0xF0) == 0xF0) {
		msbHEX.Printf(wxT("F"));
	}
	return wxString::Format(wxT("0X%s%s"), msbHEX, lsbHEX);
}

wxString decodeBinaryValue(uint8_t data) {
	int8_t bit0 = 0, bit1 = 0, bit2 = 0, bit3 = 0, bit4 = 0, bit5 = 0, bit6 = 0, bit7 = 0;
	if (data & (0x01 << 0)) bit0 = 1;
	if (data & (0x01 << 1)) bit1 = 1;
	if (data & (0x01 << 2)) bit2 = 1;
	if (data & (0x01 << 3)) bit3 = 1;
	if (data & (0x01 << 4)) bit4 = 1;
	if (data & (0x01 << 5)) bit5 = 1;
	if (data & (0x01 << 6)) bit6 = 1;
	if (data & (0x01 << 7)) bit7 = 1;
	return wxString::Format(wxT("0b%d%d%d%d%d%d%d%d"), bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0);
}
