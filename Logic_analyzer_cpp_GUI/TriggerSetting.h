#pragma once
#include "wx/wx.h"
#include "Channel.h"

enum TriggerSettingIDs{
	TRIGGER_CHOICE_PANEL_ID = 200,
	TRIGGER_ENABLE_CHANNEL1_COMBO_ID = 201,
	TRIGGER_ENABLE_CHANNEL2_COMBO_ID = 202,
	TRIGGER_ENABLE_CHANNEL3_COMBO_ID = 203,
	TRIGGER_ENABLE_CHANNEL4_COMBO_ID = 204,
	TRIGGER_ENABLE_CHANNEL5_COMBO_ID = 205,
	TRIGGER_ENABLE_CHANNEL6_COMBO_ID = 206,
	TRIGGER_ENABLE_CHANNEL7_COMBO_ID = 207,
	TRIGGER_ENABLE_CHANNEL8_COMBO_ID = 208
};

class ConfigureTriggerDialog : public wxDialog {
private:
	DECLARE_EVENT_TABLE()
	Channel* channelptr = nullptr;
public:
	ConfigureTriggerDialog(wxWindow* parent, wxWindowID id, const wxString& title, Channel*);
	void updateTriggerChoicePanel(wxUpdateUIEvent&);
	void updateChannel1Combo(wxUpdateUIEvent&);
	void updateChannel2Combo(wxUpdateUIEvent&);
	void updateChannel3Combo(wxUpdateUIEvent&);
	void updateChannel4Combo(wxUpdateUIEvent&);
	void updateChannel5Combo(wxUpdateUIEvent&);
	void updateChannel6Combo(wxUpdateUIEvent&);
	void updateChannel7Combo(wxUpdateUIEvent&);
	void updateChannel8Combo(wxUpdateUIEvent&);
	
	wxComboBox* triggerTypeCombo = nullptr;

	wxComboBox* channel1EnableTriggerCombo = nullptr;
	wxComboBox* channel2EnableTriggerCombo = nullptr;
	wxComboBox* channel3EnableTriggerCombo = nullptr;
	wxComboBox* channel4EnableTriggerCombo = nullptr;
	wxComboBox* channel5EnableTriggerCombo = nullptr;
	wxComboBox* channel6EnableTriggerCombo = nullptr;
	wxComboBox* channel7EnableTriggerCombo = nullptr;
	wxComboBox* channel8EnableTriggerCombo = nullptr;

	wxComboBox* channel1TriggerOnCombo = nullptr;
	wxComboBox* channel2TriggerOnCombo = nullptr;
	wxComboBox* channel3TriggerOnCombo = nullptr;
	wxComboBox* channel4TriggerOnCombo = nullptr;
	wxComboBox* channel5TriggerOnCombo = nullptr;
	wxComboBox* channel6TriggerOnCombo = nullptr;
	wxComboBox* channel7TriggerOnCombo = nullptr;
	wxComboBox* channel8TriggerOnCombo = nullptr;
};