#pragma once
#include "wx/wx.h"
#include "Channel.h"

enum EnableChannelFlag {
	CHANNEL1_CHECKBOX_ID = 300,
	CHANNEL2_CHECKBOX_ID = 301,
	CHANNEL3_CHECKBOX_ID = 302,
	CHANNEL4_CHECKBOX_ID = 303,
	CHANNEL5_CHECKBOX_ID = 304,
	CHANNEL6_CHECKBOX_ID = 305,
	CHANNEL7_CHECKBOX_ID = 306,
	CHANNEL8_CHECKBOX_ID = 307,
	ENABLE_CHANNEL_OK_BUTTON_ID  =308,
	ENABLE_CHANNEL_CANCEL_BUTTON_ID = 309
};

class EnableChannelDialog : public wxDialog {
private:
	//DECLARE_EVENT_TABLE()
	Channel* channelptr = nullptr;
public:
	EnableChannelDialog(wxWindow* parent, wxWindowID id, const wxString& title, Channel*);
	//void clickCheckBox1(wxCommandEvent&);
	//void clickCheckBox2(wxCommandEvent&);
	//void clickCheckBox3(wxCommandEvent&);
	//void clickCheckBox4(wxCommandEvent&);
	//void clickCheckBox5(wxCommandEvent&);
	//void clickCheckBox6(wxCommandEvent&);
	//void clickCheckBox7(wxCommandEvent&);
	//void clickCheckBox8(wxCommandEvent&);

	wxCheckBox* EnableChannel1CheckBox = nullptr;
	wxCheckBox* EnableChannel2CheckBox = nullptr;
	wxCheckBox* EnableChannel3CheckBox = nullptr;
	wxCheckBox* EnableChannel4CheckBox = nullptr;
	wxCheckBox* EnableChannel5CheckBox = nullptr;
	wxCheckBox* EnableChannel6CheckBox = nullptr;
	wxCheckBox* EnableChannel7CheckBox = nullptr;
	wxCheckBox* EnableChannel8CheckBox = nullptr;
};