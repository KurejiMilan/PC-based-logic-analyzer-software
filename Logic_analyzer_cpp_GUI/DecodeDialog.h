#pragma once
#include "wx/wx.h"
#include "Channel.h"

enum DECODE_DIALOG_ID {
	PROTOCOL_LIST_ID = 500,
	PARALLEL_COMBO_CHANNEL1_ID = 510,
	PARALLEL_COMBO_CHANNEL2_ID = 510,
	PARALLEL_COMBO_CHANNEL3_ID = 510,
	PARALLEL_COMBO_CHANNEL4_ID = 510,
	PARALLEL_COMBO_CHANNEL5_ID = 510,
	PARALLEL_COMBO_CHANNEL6_ID = 510,
	PARALLEL_COMBO_CHANNEL7_ID = 510,
	PARALLEL_COMBO_CHANNEL8_ID = 510,
};

class DecodeDialog : public wxDialog {
	DECLARE_EVENT_TABLE()
public:
	DecodeDialog(wxWindow* parent, wxWindowID id, const wxString& title, Channel* cPtr);
	void updateDecodeConfig(wxCommandEvent&);
	wxComboBox* protocolListComboBox = nullptr;

	// temp just for test
	Channel* channelPtr = nullptr;
	
	wxPanel *i2c = nullptr, *parallel = nullptr, *uart = nullptr, *spi = nullptr;
	wxComboBox* dataline1Combo = nullptr, * dataline2Combo = nullptr, * dataline3Combo = nullptr, * dataline4Combo = nullptr, * dataline5Combo = nullptr, * dataline6Combo = nullptr, * dataline7Combo = nullptr, * dataline8Combo = nullptr;
	wxComboBox *UARTTxCombo = nullptr, *UARTRxCombo = nullptr, *UARTBuadCombo = nullptr, *UARTFrameBitsCombo = nullptr, *UARTParityCombo=nullptr, *UARTStopBitsCombo=nullptr;
	wxComboBox *I2CSdaCombo = nullptr, *I2CSclCombo = nullptr;
	wxComboBox *SPICsCombo = nullptr, * SPISckCombo = nullptr, * SPIMosiCombo = nullptr, * SPIMisoCombo = nullptr;
};