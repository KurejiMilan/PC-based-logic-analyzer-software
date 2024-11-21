#pragma once
#include "wx/wx.h"
#include "WaveCanvas.h"
#include "Channel.h"
#include "EnableChannel.h"
#include "TriggerSetting.h"
#include "LabelCanvas.h"
#include "Serial.h"
#include "PortDialog.h"
#include "DecodeDialog.h"

enum MainClassIDs{
	ENABLE_CHANNEL_MENU_ID = 100,
	CONFIGURE_TRIGGER_MENU_ID = 101,
	MANAGE_PORT_MENU_ID = 102,
	DECODE_MENU_ID = 103,
	SampleID = 110,
	SampleRamSizeID = 111,
	StartSampleID = 120
};

class MainFrame :public wxFrame 
{
public:
	MainCanvas* wave = nullptr;
	ChannelLabel* waveLabel = nullptr;
	Channel* channelInfo = nullptr;
	SerialPort* port = nullptr;

	MainFrame(const wxString&);
	void startSample(wxCommandEvent&);
	void displayEnableChannelDialog(wxCommandEvent&);
	void displayConfigureTriggerDialog(wxCommandEvent&);
	void displayManagePortDialog(wxCommandEvent&);
	void displayDecodeSettingDialog(wxCommandEvent&);
	void selectSampleRate(wxCommandEvent&);
	void selectSampleSize(wxCommandEvent&);
	void zoomIn(wxCommandEvent&);
	void zoomOut(wxCommandEvent&);
	void forward(wxCommandEvent&);
	void backward(wxCommandEvent&);
	uint8_t getChannelNumber(wxString);

	wxComboBox* samplerate = nullptr;
	wxComboBox* sampleRamSize = nullptr;
private:
	// this class handles events
	DECLARE_EVENT_TABLE()
};