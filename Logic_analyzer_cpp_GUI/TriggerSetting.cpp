#include "TriggerSetting.h"

BEGIN_EVENT_TABLE(ConfigureTriggerDialog, wxDialog)
	EVT_UPDATE_UI(TRIGGER_CHOICE_PANEL_ID, ConfigureTriggerDialog::updateTriggerChoicePanel)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL1_COMBO_ID, ConfigureTriggerDialog::updateChannel1Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL2_COMBO_ID, ConfigureTriggerDialog::updateChannel2Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL3_COMBO_ID, ConfigureTriggerDialog::updateChannel3Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL4_COMBO_ID, ConfigureTriggerDialog::updateChannel4Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL5_COMBO_ID, ConfigureTriggerDialog::updateChannel5Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL6_COMBO_ID, ConfigureTriggerDialog::updateChannel6Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL7_COMBO_ID, ConfigureTriggerDialog::updateChannel7Combo)
	EVT_UPDATE_UI(TRIGGER_ENABLE_CHANNEL8_COMBO_ID, ConfigureTriggerDialog::updateChannel8Combo)
END_EVENT_TABLE()

ConfigureTriggerDialog::ConfigureTriggerDialog(wxWindow* parent, wxWindowID id, const wxString& title, Channel* cptr)	: wxDialog(parent, id, title, wxPoint(420, 200))
{
	channelptr = cptr;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	SetMinSize(wxSize(600, 20));
	wxStaticText* infoText = new wxStaticText(this, wxID_ANY, wxT("If no Trigger condition is provided, the data acquisition will start on clicking acquire button."));
	mainSizer->Add(infoText, 0, wxALL, 5);
	wxArrayString triggerChoice;
	triggerChoice.Add(wxT("None"));
	triggerChoice.Add(wxT("Edge Trigger"));
	triggerChoice.Add(wxT("Word Trigger"));

	triggerTypeCombo = new wxComboBox(this, wxID_ANY, wxT("None"), wxDefaultPosition, wxSize(110, 30), triggerChoice, wxCB_READONLY);
	
	mainSizer->Add(triggerTypeCombo, 0, wxALL, 5);

	wxPanel* triggerOption = new wxPanel(this, TRIGGER_CHOICE_PANEL_ID);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* info = new wxStaticText(triggerOption,wxID_ANY, wxT("Set the appropiate trigger options"));
	panelSizer->Add(info,0, wxLEFT|wxBOTTOM, 5);

	wxBoxSizer* channel1TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel2TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel3TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel4TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel5TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel6TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel7TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* channel8TriggerConfigure = new wxBoxSizer(wxHORIZONTAL);
	
	wxArrayString enableTriggerChannel;
	enableTriggerChannel.Add(wxT("Disable"));
	enableTriggerChannel.Add(wxT("Enable"));

	wxArrayString TriggerOn;
	TriggerOn.Add(wxT("1/Rising edge"));
	TriggerOn.Add(wxT("0/Falling edge"));

	// Channel 1
	wxStaticText* channel1Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 1"));
	channel1EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL1_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel1TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);
	
	channel1TriggerConfigure->Add(channel1Lebel, 1,wxLEFT|wxTOP, 5);
	channel1TriggerConfigure->AddStretchSpacer(1);
	channel1TriggerConfigure->Add(channel1EnableTriggerCombo, 0, wxLEFT, 5);
	channel1TriggerConfigure->Add(channel1TriggerOnCombo,0, wxLEFT|wxRIGHT, 5);

	// Channel 2
	wxStaticText* channel2Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 2"));
	channel2EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL2_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel2TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel2TriggerConfigure->Add(channel2Lebel, 1, wxLEFT | wxTOP, 5);
	channel2TriggerConfigure->AddStretchSpacer(1);
	channel2TriggerConfigure->Add(channel2EnableTriggerCombo, 0, wxLEFT, 5);
	channel2TriggerConfigure->Add(channel2TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	// Channel 3
	wxStaticText* channel3Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 3"));
	channel3EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL3_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel3TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel3TriggerConfigure->Add(channel3Lebel, 1, wxLEFT | wxTOP, 5);
	channel3TriggerConfigure->AddStretchSpacer(1);
	channel3TriggerConfigure->Add(channel3EnableTriggerCombo, 0, wxLEFT, 5);
	channel3TriggerConfigure->Add(channel3TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	// Channel 4
	wxStaticText* channel4Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 4"));
	channel4EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL4_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel4TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel4TriggerConfigure->Add(channel4Lebel, 1, wxLEFT | wxTOP, 5);
	channel4TriggerConfigure->AddStretchSpacer(1);
	channel4TriggerConfigure->Add(channel4EnableTriggerCombo, 0, wxLEFT, 5);
	channel4TriggerConfigure->Add(channel4TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	// Channel 5
	wxStaticText* channel5Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 5"));
	channel5EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL5_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel5TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel5TriggerConfigure->Add(channel5Lebel, 1, wxLEFT | wxTOP, 5);
	channel5TriggerConfigure->AddStretchSpacer(1);
	channel5TriggerConfigure->Add(channel5EnableTriggerCombo, 0, wxLEFT, 5);
	channel5TriggerConfigure->Add(channel5TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	// Channel 6
	wxStaticText* channel6Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 6"));
	channel6EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL6_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel6TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel6TriggerConfigure->Add(channel6Lebel, 1, wxLEFT | wxTOP, 5);
	channel6TriggerConfigure->AddStretchSpacer(1);
	channel6TriggerConfigure->Add(channel6EnableTriggerCombo, 0, wxLEFT, 5);
	channel6TriggerConfigure->Add(channel6TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	// Channel 7
	wxStaticText* channel7Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 7"));
	channel7EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL7_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel7TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel7TriggerConfigure->Add(channel7Lebel, 1, wxLEFT | wxTOP, 5);
	channel7TriggerConfigure->AddStretchSpacer(1);
	channel7TriggerConfigure->Add(channel7EnableTriggerCombo, 0, wxLEFT, 5);
	channel7TriggerConfigure->Add(channel7TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	// Channel 8
	wxStaticText* channel8Lebel = new wxStaticText(triggerOption, wxID_ANY, wxT("Channel 8"));
	channel8EnableTriggerCombo = new wxComboBox(triggerOption, TRIGGER_ENABLE_CHANNEL8_COMBO_ID, wxT("Disable"), wxDefaultPosition, wxSize(110, 30), enableTriggerChannel, wxCB_READONLY);
	channel8TriggerOnCombo = new wxComboBox(triggerOption, wxID_ANY, wxT("1/Rising edge"), wxDefaultPosition, wxSize(110, 30), TriggerOn, wxCB_READONLY);

	channel8TriggerConfigure->Add(channel8Lebel, 1, wxLEFT | wxTOP, 5);
	channel8TriggerConfigure->AddStretchSpacer(1);
	channel8TriggerConfigure->Add(channel8EnableTriggerCombo, 0, wxLEFT, 5);
	channel8TriggerConfigure->Add(channel8TriggerOnCombo, 0, wxLEFT | wxRIGHT, 5);

	panelSizer->Add(channel1TriggerConfigure);
	panelSizer->Add(channel2TriggerConfigure);
	panelSizer->Add(channel3TriggerConfigure);
	panelSizer->Add(channel4TriggerConfigure);
	panelSizer->Add(channel5TriggerConfigure);
	panelSizer->Add(channel6TriggerConfigure);
	panelSizer->Add(channel7TriggerConfigure);
	panelSizer->Add(channel8TriggerConfigure);

	mainSizer->Add(triggerOption);
	
	wxButton* okayButton = new wxButton(this, wxID_OK, wxT("Okay"));
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	wxBoxSizer* buttonsizer = new wxBoxSizer(wxHORIZONTAL);
	buttonsizer->Add(cancelButton, 0, wxBOTTOM, 5);
	buttonsizer->Add(okayButton, 0, wxRIGHT | wxLEFT | wxBOTTOM, 5);
	mainSizer->Add(buttonsizer, wxEXPAND|wxALIGN_RIGHT, 5);

	triggerOption->SetSizer(panelSizer);
	triggerOption->Fit();

	SetSizer(mainSizer);
	Fit();
}

void ConfigureTriggerDialog::updateTriggerChoicePanel(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("None");
	if (str.compare(triggerTypeCombo->GetValue()) != 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel1Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel2Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel3Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel4Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel5Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel6Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel7Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}

void ConfigureTriggerDialog::updateChannel8Combo(wxUpdateUIEvent& event)
{
	wxString str;
	str.Printf("Edge Trigger");
	if (str.compare(triggerTypeCombo->GetValue()) == 0) {
		event.Enable(TRUE);
	}
	else {
		event.Enable(FALSE);
	}
}
