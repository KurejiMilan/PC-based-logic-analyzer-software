#pragma once
#include "wx/wx.h"
#include "Channel.h"

class ChannelLabel : public wxPanel {
public:
	ChannelLabel(wxFrame*, Channel*);
	void onPaint(wxPaintEvent&);
	void resize(wxSizeEvent&);
	void redraw();
	Channel* channelptr = nullptr;
	int maxheight = 0;
private:
	DECLARE_EVENT_TABLE()
};