#include "EnableChannel.h"

/*BEGIN_EVENT_TABLE(EnableChannelDialog, wxDialog)
	EVT_CHECKBOX(CHANNEL1_CHECKBOX_ID, EnableChannelDialog::clickCheckBox1)
	EVT_CHECKBOX(CHANNEL2_CHECKBOX_ID, EnableChannelDialog::clickCheckBox2)
	EVT_CHECKBOX(CHANNEL3_CHECKBOX_ID, EnableChannelDialog::clickCheckBox3)
	EVT_CHECKBOX(CHANNEL4_CHECKBOX_ID, EnableChannelDialog::clickCheckBox4)
	EVT_CHECKBOX(CHANNEL5_CHECKBOX_ID, EnableChannelDialog::clickCheckBox5)
	EVT_CHECKBOX(CHANNEL6_CHECKBOX_ID, EnableChannelDialog::clickCheckBox6)
	EVT_CHECKBOX(CHANNEL7_CHECKBOX_ID, EnableChannelDialog::clickCheckBox7)
	EVT_CHECKBOX(CHANNEL8_CHECKBOX_ID, EnableChannelDialog::clickCheckBox8)
END_EVENT_TABLE()*/

EnableChannelDialog::EnableChannelDialog(wxWindow* parent, wxWindowID id, const wxString& title, Channel* ptr):wxDialog(parent, id, title, wxPoint(420, 200))
{
	channelptr = ptr;

	wxBoxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
	SetMinSize(wxSize(600, 170));
	wxStaticText* enablechannelinfotext = new wxStaticText(this, wxID_ANY, wxT("Enable the appropriate channels by selecting the checkbox."));
	mainsizer->AddSpacer(10);
	wxBoxSizer* textSizer = new wxBoxSizer(wxHORIZONTAL);
	textSizer->AddSpacer(20);
	textSizer->Add(enablechannelinfotext);
	mainsizer->Add(textSizer);
	mainsizer->AddSpacer(20);
	wxBoxSizer* checkboxsizer = new wxBoxSizer(wxHORIZONTAL);

	EnableChannel1CheckBox = new wxCheckBox(this, CHANNEL1_CHECKBOX_ID, wxT("Channel 1"));
	EnableChannel2CheckBox = new wxCheckBox(this, CHANNEL2_CHECKBOX_ID, wxT("Channel 2"));
	EnableChannel3CheckBox = new wxCheckBox(this, CHANNEL3_CHECKBOX_ID, wxT("Channel 3"));
	EnableChannel4CheckBox = new wxCheckBox(this, CHANNEL4_CHECKBOX_ID, wxT("Channel 4"));
	EnableChannel5CheckBox = new wxCheckBox(this, CHANNEL5_CHECKBOX_ID, wxT("Channel 5"));
	EnableChannel6CheckBox = new wxCheckBox(this, CHANNEL6_CHECKBOX_ID, wxT("Channel 6"));
	EnableChannel7CheckBox = new wxCheckBox(this, CHANNEL7_CHECKBOX_ID, wxT("Channel 7"));
	EnableChannel8CheckBox = new wxCheckBox(this, CHANNEL8_CHECKBOX_ID, wxT("Channel 8"));

	EnableChannel1CheckBox->SetValue(channelptr->active8ChannelMask & 0b00000001);
	EnableChannel2CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 1));
	EnableChannel3CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 2));
	EnableChannel4CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 3));
	EnableChannel5CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 4));
	EnableChannel6CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 5));
	EnableChannel7CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 6));
	EnableChannel8CheckBox->SetValue(channelptr->active8ChannelMask & (0b00000001 << 7));
	
	checkboxsizer->AddSpacer(20);
	checkboxsizer->Add(EnableChannel1CheckBox);
	checkboxsizer->Add(EnableChannel2CheckBox);
	checkboxsizer->Add(EnableChannel3CheckBox);
	checkboxsizer->Add(EnableChannel4CheckBox);
	checkboxsizer->Add(EnableChannel5CheckBox);
	checkboxsizer->Add(EnableChannel6CheckBox);
	checkboxsizer->Add(EnableChannel7CheckBox);
	checkboxsizer->Add(EnableChannel8CheckBox);
	checkboxsizer->AddSpacer(20);

	mainsizer->Add(checkboxsizer);
	mainsizer->AddSpacer(30);
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	wxButton* okButton = new wxButton(this, wxID_OK, wxT("Okay"));
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->AddSpacer(220);
	buttonSizer->Add(cancelButton);
	buttonSizer->AddSpacer(10);
	buttonSizer->Add(okButton);

	mainsizer->Add(buttonSizer);
	SetSizer(mainsizer);
	Fit();
}

/*
void EnableChannelDialog::clickCheckBox1(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 0);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 0);
}

void EnableChannelDialog::clickCheckBox2(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 1);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 1);
}

void EnableChannelDialog::clickCheckBox3(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 2);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 2);
}

void EnableChannelDialog::clickCheckBox4(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 3);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 3);
}

void EnableChannelDialog::clickCheckBox5(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 4);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 4);
}

void EnableChannelDialog::clickCheckBox6(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 5);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 5);
}

void EnableChannelDialog::clickCheckBox7(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 6);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 6);
}

void EnableChannelDialog::clickCheckBox8(wxCommandEvent& e)
{
	if (e.IsChecked()) channelptr->active8ChannelMask |= (0b00000001 << 7);
	else channelptr->active8ChannelMask &= ~(0b00000001 << 7);
}
*/
