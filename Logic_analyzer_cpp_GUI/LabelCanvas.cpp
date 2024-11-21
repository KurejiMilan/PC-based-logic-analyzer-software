#include "LabelCanvas.h"

BEGIN_EVENT_TABLE(ChannelLabel, wxPanel)
	EVT_PAINT(ChannelLabel::onPaint)
	EVT_SIZE(ChannelLabel::resize)
END_EVENT_TABLE()


ChannelLabel::ChannelLabel(wxFrame* parent, Channel* chptr) : wxPanel(parent, wxID_ANY)
{
	channelptr = chptr;
	SetBackgroundColour(*wxWHITE);
}

void ChannelLabel::onPaint(wxPaintEvent& e)
{
	wxPaintDC dc(this);
	wxSize sz = dc.GetSize();
	wxString label;
	SetBackgroundColour(wxColor(85, 150, 250 ,98));
	wxColor channelColor[8];
	channelColor[0].Set(255, 23, 68);		//red
	channelColor[1].Set(213, 0, 249);		//purple
	channelColor[2].Set(101, 31, 255);		//deep purple
	channelColor[3].Set(41, 121, 255);		//blue
	channelColor[4].Set(0, 229, 255, 100);	//cyan
	channelColor[5].Set(255, 234, 0);		//yellow
	channelColor[6].Set(118, 255, 3);		//lime
	channelColor[7].Set(255, 145, 0);		//orange
	wxPen boarder(wxColor(85, 150, 250, 98), 1, wxPENSTYLE_SOLID);
	wxBrush labelBackgroud(wxColor(18, 18, 18), wxBRUSHSTYLE_SOLID);		//40, 37, 37, 16		//wxColor(120, 114, 118),
	wxFont font(10, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	dc.SetFont(font);
	dc.SetBackgroundMode(wxTRANSPARENT);
	dc.SetPen(boarder);
	dc.SetBrush(labelBackgroud);

	uint16_t labelSpacing = 0;
	if ((channelptr->totalActiveChannel + channelptr->decodedChannel) >= 8) {
		labelSpacing = ((maxheight - 15 - 20) / (channelptr->totalActiveChannel + channelptr->decodedChannel));
	}
	else {
		labelSpacing = (maxheight - 15 - 20) / 8;
	}

	uint8_t z = 0;
	for (uint8_t i = 0, j = 0; i < 8; i++) 
	{
		if (channelptr->active8ChannelMask & (0b00000001 << i)) {
			dc.DrawRoundedRectangle(0, labelSpacing*j+15, sz.x, labelSpacing, 3);
			dc.SetTextForeground(channelColor[i].GetRGB());
			label.Printf(wxT("Channel %d"), i);
			wxSize textSize = dc.GetTextExtent(label);
			dc.DrawText(label, wxPoint( (sz.x / 2) - (textSize.x / 2), labelSpacing / 2 - textSize.y / 2 + ((j * labelSpacing) + 15)));
			j++;
			z++;
		}
	}

	if (channelptr->SelectedProtocolDecoder != 0) {
		wxString channelName;
		if (channelptr->SelectedProtocolDecoder == PARALLEL) {
			dc.DrawRoundedRectangle(0, labelSpacing * z + 15, sz.x, labelSpacing, 3);	
			dc.SetTextForeground(*wxWHITE);
			channelName.Printf(wxT("PARALLEL"));
			wxSize textSize = dc.GetTextExtent(channelName);
			dc.DrawText(wxString::Format(wxT("PARALLEL")), wxPoint((sz.x/2)-(textSize.x/ 2) ,  labelSpacing/2 -textSize.y/2 +((z * labelSpacing) + 15)  ));
		}
		else if (channelptr->SelectedProtocolDecoder == SPI) {
			// for MOSI
			wxSize textSize;
			dc.SetTextForeground(wxColor(49, 206, 142));
			dc.DrawRoundedRectangle(0, labelSpacing * z + 15, sz.x, labelSpacing, 3);
			channelName.Printf(wxT("MOSI"));
			textSize = dc.GetTextExtent(channelName);
			dc.DrawText(wxString::Format(wxT("MOSI")), wxPoint((sz.x / 2) - (textSize.x / 2), labelSpacing / 2 - textSize.y / 2 + ((z * labelSpacing) + 15)));
			z++;
			
			// for MISO
			dc.DrawRoundedRectangle(0, labelSpacing * z + 15, sz.x, labelSpacing, 3);
			channelName.Printf(wxT("MISO"));
			textSize = dc.GetTextExtent(channelName);
			dc.DrawText(wxString::Format(wxT("MISO")), wxPoint((sz.x / 2) - (textSize.x / 2), labelSpacing / 2 - textSize.y / 2 + ((z * labelSpacing) + 15)));
		}
		else if (channelptr->SelectedProtocolDecoder == I2C) {
			// for i2c
			wxSize textSize;
			dc.SetTextForeground(wxColor(247, 98, 8));
			dc.DrawRoundedRectangle(0, labelSpacing * z + 15, sz.x, labelSpacing, 3);
			channelName.Printf(wxT("I2C"));
			textSize = dc.GetTextExtent(channelName);
			dc.DrawText(channelName, wxPoint((sz.x / 2) - (textSize.x / 2), labelSpacing / 2 - textSize.y / 2 + ((z * labelSpacing) + 15)));
		}
	}
}

void ChannelLabel::resize(wxSizeEvent& e)
{
	wxSize sz = GetSize();
	if (sz.y > maxheight) maxheight = sz.y;
	Refresh();
	Update();
	e.Skip();
}

void ChannelLabel::redraw()
{
	Refresh();
	Update();
}
