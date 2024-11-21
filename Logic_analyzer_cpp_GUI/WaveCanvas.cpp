/*color code for background color https://web.njit.edu/~walsh/rgb.html*/

#include "WaveCanvas.h"
#include "wx/colour.h"

BEGIN_EVENT_TABLE(MainCanvas, wxPanel)
	EVT_PAINT(MainCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(MainCanvas::backgroundPaint)
	EVT_SIZE(MainCanvas::resize)
	EVT_SCROLL(MainCanvas::scrollHandler)
END_EVENT_TABLE()

MainCanvas::MainCanvas(wxFrame* p, Channel* cptr) : wxPanel(p, wxID_ANY)
{
	//wxColor c(11,15,46,18);
	//wxColor c(24, 37, 46, 18);
	wxColor c(40, 37, 37, 16);
	channelptr = cptr;
	SetBackgroundColour(*wxBLACK);
	scrollbar = new wxScrollBar(this, wxID_ANY, wxPoint(0, 0), wxSize(0,0), wxSB_HORIZONTAL);
	scrollbar->SetScrollbar(0, 5, channelptr->blockTransferSize * 256, 800);
}

void MainCanvas::OnPaint(wxPaintEvent& e)
{
	wxBufferedPaintDC dc(this);
	
	wxSize sz = dc.GetSize();
	wxColor bgC(18, 18, 18);				//18;18;18	40, 37, 37, 16
	dc.SetPen(wxPen(bgC));
	dc.SetBrush(wxBrush(bgC));
	dc.DrawRectangle(0, 0, sz.x, sz.y);

	wxPen graph(*wxWHITE,1, wxPENSTYLE_SOLID);
	dc.SetPen(graph);
	/*
		draws the grids and the timescale 
	*/
	dc.DrawLine(0, 15, sz.x, 15);

	timeLineFontSetting(&dc);
	wxString timelineString;
	int timelineTime = 0;			//used to store the time
	//uint8_t timedividor = 0;						// 0 means NS 1 means US 2 means MS 3 means S
	wxFont timelineFont(8, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	dc.SetFont(timelineFont);
	float displayTime = (float)sampleIndex*channelptr->timeScale;
	do {
		dc.DrawLine(timelineTime, 15, timelineTime, 4);
		if ((channelptr->sampleRateIndex == 0) | (channelptr->sampleRateIndex == 1) | (channelptr->sampleRateIndex == 2)) {
			if (displayTime > 1000000000) {
				timelineString.Printf(wxT("%.2fs"), displayTime/ 1000000000);
			}
			else if (displayTime > 1000000) {
				timelineString.Printf(wxT("%.2fms"), displayTime / 1000000);
			}
			else if (displayTime > 1000) {
				timelineString.Printf(wxT("%.2fus"), displayTime / 1000);
			}
			else {
				timelineString.Printf(wxT("%.2fns"), displayTime);
			}
		}
		else {
			if (displayTime > 1000000) {
				timelineString.Printf(wxT("%.2fs"), displayTime / 1000000);
			}
			else if (displayTime > 1000) {
				timelineString.Printf(wxT("%.2fms"), displayTime / 1000);
			}
			else {
				timelineString.Printf(wxT("%.2fus"), displayTime);
			}
		}
		//timelineString.Printf(wxT("%.2fus"), timelineTime);
		dc.DrawText(timelineString, timelineTime + 1, 2);

		//graph.SetStyle(wxPENSTYLE_LONG_DASH);
		/*graph.SetColour(wxColor(197, 198, 208));
		dc.SetPen(graph);
		if (timelineTime != 0) {
			uint16_t tempI = 0;
			do {
				if(tempI %10 == 0)dc.DrawLine(timelineTime, 15 + (tempI*8), timelineTime, 15 + ((tempI+1)*8));
				tempI++;
			} while ((15+(tempI*10)) < sz.y);
		}*/
		timelineTime += zoomFactor[zoomIndex] * timeLine[zoomIndex];
		displayTime = displayTime + timelineTime * channelptr->timeScale;
		//graph.SetStyle(wxPENSTYLE_SOLID);
		graph.SetColour(*wxWHITE);
		dc.SetPen(graph);
	} while (timelineTime< sz.x);

	wxBrush brush(*wxRED, wxBRUSHSTYLE_TRANSPARENT);
	wxFont font(6, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	
	dc.SetFont(font);
	dc.SetBackgroundMode(wxTRANSPARENT);
	dc.SetTextForeground(*wxWHITE);
	dc.SetBrush(brush);

	/*setting the color used for the 8 channels*/
	wxColor channelColor[8];
	channelColor[0].Set(255, 23, 68);		//red	255, 23, 68
	channelColor[1].Set(213, 0, 249);		//purple=213, 0, 249 neone purple = 199,36,177
	channelColor[2].Set(101, 31, 255);		//deep purple
	channelColor[3].Set(41, 121, 255);		//blue
	channelColor[4].Set(0, 229, 255, 100);	//cyan
	channelColor[5].Set(250, 253, 15);		//yellow 255, 234, 0
	channelColor[6].Set(118, 255, 3);		//lime
	channelColor[7].Set(255, 165, 0);		//orange  255, 145, 0
	
	wxPen channelPen(*wxWHITE, 2);
	channelPen.SetCap(wxCAP_BUTT);
	
	/*This section deals with drawing the sampled data to the screen*/

	//samples is an array of test Data nothing more
	/*
	samples = new uint8_t[65536];
	uint8_t tempsample = 0;

	for (unsigned int i = 0, z=0; i < 8192; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			samples[z] = tempsample;
			z++;
			tempsample++;
		}
		//++tempsample;
	}
	*/

	uint16_t channelGap = 0;
	if ((channelptr->totalActiveChannel + channelptr->decodedChannel) >= 8) {
		channelGap = ((maxCanvasHeight - 15 - 20) / (channelptr->totalActiveChannel + channelptr->decodedChannel));
	}
	else {
		channelGap = (maxCanvasHeight - 15 - 20) / 8;
	}
	
	if (samples != nullptr) {
		int prevSampleTime = 0, currentSampleTime = 0;
		uint16_t indexOfsample = sampleIndex;
		uint8_t prevSample = samples[indexOfsample], currentSample = 0;
		do {
			indexOfsample++;
			if (indexOfsample > channelptr->blockTransferSize*256) indexOfsample = channelptr->blockTransferSize * 256-1;
			currentSample = samples[indexOfsample];
			currentSampleTime += zoomFactor[zoomIndex];

			for (uint8_t i = 0, j = 1; i < 8; i++) {
				channelPen.SetColour(channelColor[i].GetRGB());
				dc.SetPen(channelPen);
				if (channelptr->active8ChannelMask & (0b00000001 << i)) {
					if ((prevSample & (0x01 << i)) ^ (currentSample & (0x01 << i))) {
						/* XNOR operation to find if there is change in logic level state
						*/
						if (prevSample & (0x01 << i)) {
							// this means that the previous logic level was high and we would draw a falling edge at the end
							dc.DrawLine(prevSampleTime, channelGap * j + 25 - channelGap, currentSampleTime, channelGap * j + 25 - channelGap);
							dc.DrawLine(currentSampleTime, channelGap * j + 25 - channelGap, currentSampleTime, channelGap * j + 5);					//draw the falling edge

						}
						else {
							// rising edge at the end
							dc.DrawLine(prevSampleTime, channelGap * j + 5, currentSampleTime, channelGap * j + 5);
							dc.DrawLine(currentSampleTime, channelGap * j + 5, currentSampleTime, channelGap * j + 25 - channelGap);
						}
					}
					else {
						if ((prevSample & (0x01 << i)) & (currentSample & (0x01 << i))) {
							//logic state is high for both sample
							dc.DrawLine(prevSampleTime, channelGap * j + 25 - channelGap, currentSampleTime, channelGap * j + 25 - channelGap);			// these value should be tinkered more
						}
						else {
							// logic state is low for both sample
							dc.DrawLine(prevSampleTime, channelGap * j + 5, currentSampleTime, channelGap * j + 5);
						}

					}

					j++;
				}
			}
			prevSampleTime = currentSampleTime;
			prevSample = currentSample;
		} while ((currentSampleTime < sz.x) && (indexOfsample < (channelptr->blockTransferSize * 256)-1));
	}
	else {
		for (uint8_t i = 0, j = 1; i < 8; i++) {
			channelPen.SetColour(channelColor[i].GetRGB());
			dc.SetPen(channelPen);
			if (channelptr->active8ChannelMask & (0b00000001 << i)) {
				dc.DrawLine(0, channelGap * j + 5, sz.x, channelGap * j + 5);				
				j++;
			}
		}
	}

	if (samples != nullptr) {
		if (channelptr->SelectedProtocolDecoder == PARALLEL)DecodeParallel(&dc, samples, zoomFactor[zoomIndex], (15 + channelGap * channelptr->totalActiveChannel + channelGap / 2), channelGap, sampleIndex, sz.x, channelptr->decodeLineState, channelptr->blockTransferSize * 256);
		else if (channelptr->SelectedProtocolDecoder == SPI) {
			channelPen.SetColour(wxColor(49, 206, 142));
			dc.SetPen(channelPen);
			DecodeSPI(&dc, samples, zoomFactor[zoomIndex], (15 + channelGap * channelptr->totalActiveChannel + channelGap / 2), channelGap, sampleIndex, sz.x, channelptr->blockTransferSize * 256, channelptr);
		}
		else if (channelptr->SelectedProtocolDecoder == I2C) {
			channelPen.SetColour(wxColor(247, 98, 8));
			dc.SetPen(channelPen);
			DecodeI2C(&dc, samples, zoomFactor[zoomIndex], (15 + channelGap * channelptr->totalActiveChannel + channelGap / 2), channelGap, sampleIndex, sz.x, channelptr->blockTransferSize * 256, channelptr);
		}
	}
	
}

void MainCanvas::backgroundPaint(wxEraseEvent&)
{
}

void MainCanvas::resize(wxSizeEvent& e)
{
	wxSize s = GetSize();
	if (s.x > this->maxCanvasLength) {
		this->maxCanvasLength = s.x;
		scrollbar->SetSize(wxSize(s.x, 20));
	}
	if (s.y > this->maxCanvasHeight) {
		this->maxCanvasHeight = s.y;
		scrollbar->SetPosition(wxPoint(0, s.y-20));
	}
	scrollbar->SetScrollbar(sampleIndex, 5, channelptr->blockTransferSize* 256, 100);
	Refresh();
	Update();
	e.Skip();
}

void MainCanvas::redrawMainCanvas()
{
	Refresh();
	Update();
}

void MainCanvas::scrollHandler(wxScrollEvent&)
{
	sampleIndex = scrollbar->GetThumbPosition();
	if (sampleIndex > (channelptr->blockTransferSize* 256 - 7)) sampleIndex = (channelptr->blockTransferSize * 256 - 7);
	redrawMainCanvas();
}

void MainCanvas::timeLineFontSetting(wxDC* dc)
{
	wxFont font(7, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	dc->SetFont(font);
	dc->SetBackgroundMode(wxTRANSPARENT);
	dc->SetTextForeground(*wxWHITE);
}
