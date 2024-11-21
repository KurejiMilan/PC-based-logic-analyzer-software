#pragma once
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "channel.h"
#include "Decoder.h"

class MainCanvas: public wxPanel {
public:
	MainCanvas(wxFrame*, Channel*);
	void OnPaint(wxPaintEvent&);
	void backgroundPaint(wxEraseEvent&);
	void resize(wxSizeEvent&);
	void redrawMainCanvas();
	void scrollHandler(wxScrollEvent&);
	void timeLineFontSetting(wxDC*);

public:
	uint8_t* samples = nullptr;
	uint8_t zoomFactor[10] = { 1, 2, 5, 10, 20, 50, 80, 100, 150, 200 };				// this is the number of pizxels that a sample is drawn
	uint8_t timeLine[10] = {50, 25, 10, 5, 3, 2, 1, 1, 1, 1};							// this defines the number of pixels after which the time is shown on the timescale corresponds with zoomfactor
	uint8_t sampleShift[10] = {100, 100, 80, 70, 25, 15, 10, 5, 2, 1};					// number of samples to be shifed from current index i.e. added or subtracted
	uint8_t zoomIndex = 0;
	int sampleIndex = 0;
	int maxCanvasLength=0, maxCanvasHeight = 0;
	Channel* channelptr = nullptr;
	wxScrollBar* scrollbar = nullptr;
private:
	DECLARE_EVENT_TABLE();
};