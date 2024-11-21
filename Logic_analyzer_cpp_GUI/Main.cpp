#include "Main.h"
#include "wx/artprov.h"
#include "start.xpm"
#include "zoomout.xpm"
#include "zoomin.xpm"
#include "mainICON.xpm"

uint8_t READY_TO_TX = 0x01;
uint8_t READY_TO_RX = 0x02;
uint8_t READY_TO_TX_SAMPLE_RATE = 0x03;
uint8_t READY_TO_TX_SAMPLE_SIZE = 0x04;
uint8_t ACK_FROM_RX = 0xff;
uint8_t ACK_FROM_TX = 0xfe;
uint8_t TRANSMIT_DATA = 240,SAMPLE_READY = 245, CHUNK_REC_ACK = 242, SAMPLE_TRANSMIT_COMPLETE = 243;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_COMBOBOX(SampleID, MainFrame::selectSampleRate)
	EVT_COMBOBOX(SampleRamSizeID, MainFrame::selectSampleSize)
	EVT_TOOL(StartSampleID, MainFrame::startSample)
	EVT_MENU(ENABLE_CHANNEL_MENU_ID, MainFrame::displayEnableChannelDialog)
	EVT_MENU(CONFIGURE_TRIGGER_MENU_ID, MainFrame::displayConfigureTriggerDialog)
	EVT_MENU(MANAGE_PORT_MENU_ID, MainFrame::displayManagePortDialog)
	EVT_MENU(DECODE_MENU_ID, MainFrame::displayDecodeSettingDialog)
	EVT_TOOL(wxID_ZOOM_IN, MainFrame::zoomIn)
	EVT_TOOL(wxID_ZOOM_OUT, MainFrame::zoomOut)
	EVT_MENU(wxID_FORWARD, MainFrame::forward)
	EVT_MENU(wxID_BACKWARD, MainFrame::backward)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	SetIcon(wxIcon(icon));
	channelInfo = new Channel;
	port = new SerialPort;
	SetMinSize(wxSize(1200, 760));
	//Implementation of the menu
	wxMenuBar* menu = new wxMenuBar;

	wxMenu* filemenu = new wxMenu;
	wxMenu* configuremenu = new wxMenu;
	wxMenu* helpmenu = new wxMenu;
	wxMenu* viewmenu = new wxMenu;

	filemenu->Append(wxID_EXIT,wxT("E&xit\tAlt-X"),wxT("Quit this program"));
	configuremenu->Append(ENABLE_CHANNEL_MENU_ID, wxT("Enable channel"), wxT("Enable and disable the channels"));
	configuremenu->Append(CONFIGURE_TRIGGER_MENU_ID, wxT("Trigger setting"));
	configuremenu->Append(MANAGE_PORT_MENU_ID, wxT("Manage Port"));
	viewmenu->Append(DECODE_MENU_ID, wxT("Decode"), wxT("Decode protocols"));
	viewmenu->Append(wxID_FORWARD, wxT("Forward\tCtrl+Right"), wxT("Scroll forward"));
	viewmenu->Append(wxID_BACKWARD, wxT("Backward\tCtrl+Left"), wxT("Scroll backward"));
	viewmenu->Append(wxID_ZOOM_IN, wxT("Zoom In\tCtrl+Up"));
	viewmenu->Append(wxID_ZOOM_OUT, wxT("Zoom Out\tCtrl+Down"));
	helpmenu->Append(wxID_ABOUT, wxT("&About\tF1"), wxT("Show about dialog"));

	menu->Append(filemenu, wxT("File"));
	menu->Append(viewmenu, wxT("View"));
	menu->Append(configuremenu, wxT("Configure"));
	menu->Append(helpmenu, wxT("Help"));

	//attach this menubar to the frame
	SetMenuBar(menu);

	// Implementation of the toolbar
	wxBitmap startBmp(start);
	wxBitmap zoomoutBpm(zoomout);
	wxBitmap zoominBpm(zoomin);

	wxToolBar* toolbar = CreateToolBar();
	toolbar->SetBackgroundColour(*wxWHITE);			// wxColor(85, 150, 250, 98)
	toolbar->AddTool(StartSampleID, wxT("start"),startBmp);
	toolbar->SetToolShortHelp(100,wxT("Start the capture"));
	toolbar->AddSeparator();
	toolbar->AddTool(wxID_ZOOM_IN, wxT("zoom in"), zoominBpm);
	toolbar->AddTool(wxID_ZOOM_OUT, wxT("zoom out"), zoomoutBpm);
	toolbar->AddSeparator();
	//toolbar->AddTool(101, wxT("Help"), wxArtProvider::GetBitmap("wxART_HELP"));
	//toolbar->AddSeparator();
	wxArrayString sampleRates;
	sampleRates.Add(wxT("6.25 Mhz"));				// 000
	sampleRates.Add(wxT("3.125 Mhz"));				// 001
	sampleRates.Add(wxT("1.5625 Mhz"));				// 010  This is default
	sampleRates.Add(wxT("781.25 Khz"));				// 011
	sampleRates.Add(wxT("390.625 Khz"));			// 1XX

	samplerate = new wxComboBox(toolbar, SampleID, wxT("1.5625 Mhz"), wxDefaultPosition, wxSize(110,20), sampleRates, wxCB_READONLY);
	toolbar->AddControl(samplerate);
	// Realize function must be called to configure the toolbar visually and spactially 

	wxArrayString ramSize;
	
	ramSize.Add(wxT("1 KB"));
	ramSize.Add(wxT("2 KB"));
	ramSize.Add(wxT("4 KB"));
	ramSize.Add(wxT("8 KB"));
	ramSize.Add(wxT("16 KB"));
	ramSize.Add(wxT("32 KB"));
	ramSize.Add(wxT("64 KB"));

	sampleRamSize = new wxComboBox(toolbar, SampleRamSizeID, wxT("8 KB"), wxDefaultPosition, wxSize(70, 20), ramSize, wxCB_READONLY);
	toolbar->AddControl(sampleRamSize);
	toolbar->Realize();

	//create a status bar
	CreateStatusBar();
	SetStatusText(wxT("welcome to logic anazlyser view"));

	// adding drawing client area
	wave = new MainCanvas(this, channelInfo);
	waveLabel = new ChannelLabel(this, channelInfo);

	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(waveLabel,1,wxEXPAND);
	topSizer->Add(wave, 9, wxEXPAND);
	
	wxSize waveSize = wave->GetSize();
	wave->maxCanvasLength = waveSize.x;
	wave->maxCanvasHeight = waveSize.y;

	wxSize waveLabelSize = waveLabel->GetSize();
	waveLabel->maxheight = waveLabelSize.y;

	SetSizer(topSizer);
	topSizer->Fit(this);						// fit the frame to the contents
	//topSizer->SetSizeHints(this);
}

void MainFrame::startSample(wxCommandEvent&)
{
	uint8_t receivedWord = 0x00;
	uint8_t tempSample[256];
	bool drawn = false;
	if (port->isConnected()) {

		if (port->writeCommand(&TRANSMIT_DATA)) {

			port->readSerialPort(&receivedWord, 1);
			if (receivedWord == SAMPLE_READY) {

				SetStatusText(wxT("sample ready!"));	
				if (wave->samples != nullptr) {
					delete[] wave->samples;
					wave->samples = nullptr;
				}
				wave->samples = new uint8_t[channelInfo->blockTransferSize * 256];
				port->writeCommand(&TRANSMIT_DATA);
				for (int i = 0; i < channelInfo->blockTransferSize; i++) {
					port->readSerialPort(&wave->samples[(i * 256)], 256);

					if ( (wave->zoomFactor[wave->zoomIndex] * (i + 1) * 256 > wave->maxCanvasLength) && (!drawn) ) {
						wave->sampleIndex = 0;
						wave->scrollbar->SetThumbPosition(0);
						wave->redrawMainCanvas();
						drawn = true;
					}
					port->writeCommand(&CHUNK_REC_ACK);
				}

				port->readSerialPort(&receivedWord, 1);
				if (receivedWord == SAMPLE_TRANSMIT_COMPLETE) {
					wave->sampleIndex = 0;
					wave->scrollbar->SetThumbPosition(0);
					wave->redrawMainCanvas();
					SetStatusText(wxT("acquisition complete!"));
				}
				else {
					SetStatusText(wxT("ERROR!!!!!!! acquisition incomplete!"));
				}
			}
			else SetStatusText(wxT("Error in acqusition!"));
		}
		else SetStatusText(wxT("Failed to start communication with Logic analyzer!!"));
	}
}

void MainFrame::displayEnableChannelDialog(wxCommandEvent&)
{
	uint8_t enabledChannels = 0;
	EnableChannelDialog* Dialog_enableChannel = new EnableChannelDialog(this, wxID_ANY, "Enable Channel Dialog", channelInfo);
	if (Dialog_enableChannel->ShowModal() == wxID_OK) {

		if (Dialog_enableChannel->EnableChannel1CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 0);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 0);

		if (Dialog_enableChannel->EnableChannel2CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 1);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 1);

		if (Dialog_enableChannel->EnableChannel3CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 2);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 2);

		if (Dialog_enableChannel->EnableChannel4CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 3);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 3);

		if (Dialog_enableChannel->EnableChannel5CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 4);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 4);

		if (Dialog_enableChannel->EnableChannel6CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 5);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 5);

		if (Dialog_enableChannel->EnableChannel7CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 6);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 6);

		if (Dialog_enableChannel->EnableChannel8CheckBox->GetValue()) {
			channelInfo->active8ChannelMask |= (0b00000001 << 7);
			enabledChannels++;
		}else channelInfo->active8ChannelMask &= ~(0b00000001 << 7);
		channelInfo->totalActiveChannel = enabledChannels;
	}
	Dialog_enableChannel->Destroy();
	wave->redrawMainCanvas();
	waveLabel->redraw();
}

void MainFrame::displayConfigureTriggerDialog(wxCommandEvent& event)
{
	ConfigureTriggerDialog* configure_triggerDialog = new ConfigureTriggerDialog(this, wxID_ANY, wxT("Configure Trigger"), channelInfo);
	uint8_t receivedWord = 0;
	uint8_t maskWord = 0, triggerOn = 0;		  
	wxString triggerEnable, triggerRising, edge, word;

	triggerEnable.Printf(wxT("Enable"));
	triggerRising.Printf(wxT("1/Rising edge"));
	edge.Printf(wxT("Edge Trigger"));
	word.Printf(wxT("Word Trigger"));
	
	if (configure_triggerDialog->ShowModal() == wxID_OK) {
		
		if (edge.compare(configure_triggerDialog->triggerTypeCombo->GetValue()) == 0) {
			// checking if the trigger channel is enabled, if so filliping the bit to 1 to indicate it is enabled
			if (triggerEnable.compare(configure_triggerDialog->channel1EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 0;
			if (triggerEnable.compare(configure_triggerDialog->channel2EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 1;
			if (triggerEnable.compare(configure_triggerDialog->channel3EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 2;
			if (triggerEnable.compare(configure_triggerDialog->channel4EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 3;
			if (triggerEnable.compare(configure_triggerDialog->channel5EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 4;
			if (triggerEnable.compare(configure_triggerDialog->channel6EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 5;
			if (triggerEnable.compare(configure_triggerDialog->channel7EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 6;
			if (triggerEnable.compare(configure_triggerDialog->channel8EnableTriggerCombo->GetValue()) == 0) maskWord |= 1 << 7;
			channelInfo->triggerType = 1;
		}
		else if (word.compare(configure_triggerDialog->triggerTypeCombo->GetValue()) == 0) {
			maskWord = 0xff;
			channelInfo->triggerType = 2;
		}
		else channelInfo->triggerType = 0;

		if (triggerRising.compare(configure_triggerDialog->channel1TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 0;
		if (triggerRising.compare(configure_triggerDialog->channel2TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 1;
		if (triggerRising.compare(configure_triggerDialog->channel3TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 2;
		if (triggerRising.compare(configure_triggerDialog->channel4TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 3;
		if (triggerRising.compare(configure_triggerDialog->channel5TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 4;
		if (triggerRising.compare(configure_triggerDialog->channel6TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 5;
		if (triggerRising.compare(configure_triggerDialog->channel7TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 6;
		if (triggerRising.compare(configure_triggerDialog->channel8TriggerOnCombo->GetValue()) == 0) triggerOn |= 1 << 7;
		
		channelInfo->triggerMask = maskWord;
		channelInfo->triggerWord = triggerOn;
		
		//wxString msg;

		//msg.Printf(wxT("trigger mask = %d"), channelInfo->triggerMask);
		//wxMessageBox(msg, wxT("sample rate"), wxOK | wxICON_INFORMATION, this);
		//msg.Printf(wxT("trigger mask = %d"), channelInfo->triggerWord);
		///wxMessageBox(msg, wxT("sample rate"), wxOK | wxICON_INFORMATION, this);
		//msg.Printf(wxT("trigger type = %d"), channelInfo->triggerType);
		//wxMessageBox(msg, wxT("sample rate"), wxOK | wxICON_INFORMATION, this);
		if (port->isConnected()) {
			// initaite serial communication to configure trigger
			port->writeCommand(&READY_TO_TX);
			port->readSerialPort(&receivedWord, 1);

			if (receivedWord == READY_TO_RX) {
				if (channelInfo->triggerType == 1) {								
					// confiure trigger for edge trigger
					port->writeCommand(&channelInfo->triggerType);
					port->readSerialPort(&receivedWord, 1);
					if (receivedWord == ACK_FROM_RX) {
						port->writeCommand(&channelInfo->triggerMask);
						port->readSerialPort(&receivedWord, 1);
						if (receivedWord == ACK_FROM_RX) {
							port->writeCommand(&channelInfo->triggerWord);
							port->readSerialPort(&receivedWord, 1);
							if (receivedWord == ACK_FROM_RX) SetStatusText(wxT("Successfully configured triggered"));
							else SetStatusText(wxT("Failed to receive ack fom rx"));
						}
						else SetStatusText(wxT("Failed to receive ack fom rx"));
					}
					else SetStatusText(wxT("Failed to receive ack fom rx"));
					
				}

				else if (channelInfo->triggerType == 2) {
					// confiure trigger for word trigger
					port->writeCommand(&channelInfo->triggerType);
					port->readSerialPort(&receivedWord, 1);

					if (receivedWord == ACK_FROM_RX) {
						port->writeCommand(&channelInfo->triggerWord);
						port->readSerialPort(&receivedWord, 1);

						if (receivedWord == (uint8_t)ACK_FROM_RX) SetStatusText(wxT("Successfully configured triggered"));
						else SetStatusText(wxT("Failed to receive ack fom rx"));
					}
					else SetStatusText(wxT("Failed to receive ack fom rx"));
				}

				else {
					// confiure trigger for no trigger
					port->writeCommand(&channelInfo->triggerType);
					port->readSerialPort(&receivedWord, 1);
					if (receivedWord == ACK_FROM_RX) SetStatusText(wxT("success in configuring trigger"));
					else SetStatusText(wxT("Failed to receive ack from RX"));	
				}
			}
			else SetStatusText(wxT("Failed to receive ready_to_RX"));
		}
	}
	configure_triggerDialog->Destroy();
}

void MainFrame::displayManagePortDialog(wxCommandEvent&)
{
	ManagePort* managePortDialog = new ManagePort(this, wxID_ANY, wxT("Manage Port"), port);
	managePortDialog->ShowModal();
	managePortDialog->Destroy();
}

void MainFrame::displayDecodeSettingDialog(wxCommandEvent&)
{
	DecodeDialog* decodeSetting = new DecodeDialog(this, wxID_ANY, wxT("Decode"), channelInfo);
	if (decodeSetting->ShowModal() == wxID_OK) {
		wxString i2ct, spit, uartt, parallelt;
		i2ct.Printf("I2C");
		spit.Printf("SPI");
		uartt.Printf("UART");
		parallelt.Printf("Parallel");
		
		if (i2ct.compare(decodeSetting->protocolListComboBox->GetValue()) == 0) {
			channelInfo->SelectedProtocolDecoder = I2C;

			channelInfo->SCLchannel = getChannelNumber(decodeSetting->I2CSclCombo->GetValue());
			channelInfo->SDAchannel = getChannelNumber(decodeSetting->I2CSdaCombo->GetValue());

			channelInfo->decodedChannel = 1;
		}
		else if (spit.compare(decodeSetting->protocolListComboBox->GetValue()) == 0) {
			channelInfo->SelectedProtocolDecoder = SPI;

			channelInfo->SPICSchannel = getChannelNumber(decodeSetting->SPICsCombo->GetValue());
			channelInfo->SPISCKchannel = getChannelNumber(decodeSetting->SPISckCombo->GetValue());
			channelInfo->SPIMOSIchannel = getChannelNumber(decodeSetting->SPIMosiCombo->GetValue());
			channelInfo->SPIMISOchannel = getChannelNumber(decodeSetting->SPIMisoCombo->GetValue());

			channelInfo->decodedChannel = 2;
			//SetStatusText(wxString::Format(wxT("cs=%d, sck=%d, mosi=%d, miso=%d"), channelInfo->SPICSchannel, channelInfo->SPISCKchannel, channelInfo->SPIMOSIchannel, channelInfo->SPIMISOchannel));
		}
		else if (uartt.compare(decodeSetting->protocolListComboBox->GetValue()) == 0) {
			channelInfo->SelectedProtocolDecoder = UART;
			channelInfo->decodedChannel = 2;
		}
		else if (parallelt.compare(decodeSetting->protocolListComboBox->GetValue()) == 0) {
			channelInfo->SelectedProtocolDecoder = PARALLEL;
			channelInfo->decodedChannel = 1;	
		}
		else {
			channelInfo->SelectedProtocolDecoder = 0;
			channelInfo->decodedChannel = 0;
		}
		wave->redrawMainCanvas();
		waveLabel->redraw();
	}
	decodeSetting->Destroy();
}

void MainFrame::selectSampleRate(wxCommandEvent& event)
{
	//wxString msg;
	wxString rate0, rate1, rate2, rate3, rate4;
	uint8_t receivedWord = 0x00;

	rate0.Printf(wxT("6.25 Mhz"));
	rate1.Printf(wxT("3.125 Mhz"));
	rate2.Printf(wxT("1.5625 Mhz"));
	rate3.Printf(wxT("781.25 Khz"));
	rate4.Printf(wxT("390.625 Khz"));

	if (rate0.compare(samplerate->GetValue()) == 0) {
		channelInfo->sampleRateIndex = 0;
		channelInfo->timeScale = 160.00;
	}
	else if (rate1.compare(samplerate->GetValue()) == 0) {
		channelInfo->sampleRateIndex = 1;
		channelInfo->timeScale = 320.00;
	}
	else if (rate2.compare(samplerate->GetValue()) == 0) {
		channelInfo->sampleRateIndex = 2;
		channelInfo->timeScale = 640.0;
	}
	else if (rate3.compare(samplerate->GetValue()) == 0) {
		channelInfo->sampleRateIndex = 3;
		channelInfo->timeScale = 1.28;
	}
	else {
		channelInfo->sampleRateIndex = 4;
		channelInfo->timeScale = 2.56;
	}
	if (port->isConnected()) {
		port->writeCommand(&READY_TO_TX_SAMPLE_RATE);
		while (!port->readSerialPort(&receivedWord, 1));

		if (receivedWord == READY_TO_RX) {
			port->writeCommand(&channelInfo->sampleRateIndex);
			port->readSerialPort(&receivedWord, 1);
			if (receivedWord == ACK_FROM_RX) SetStatusText(wxT("Complete setting sample rate!"));
			else SetStatusText(wxT("Failed to receive ACK from analyzer on select sample rate!"));
		}
		else SetStatusText(wxT("Failed to receive ready from analyzer on select sample rate!"));
	}
	else SetStatusText(wxT("Connect to serial port!"));
	//msg.Printf(wxT("Selected sample rate: %s, index = %d"), samplerate->GetValue(), channelInfo->sampleRateIndex);
	//wxMessageBox(msg, wxT("sample rate"), wxOK | wxICON_INFORMATION, this);

}

void MainFrame::selectSampleSize(wxCommandEvent&)
{
	wxString size0, size1, size2, size3, size4, size5, size6;
	uint8_t receivedWord = 0x00;

	size0.Printf(wxT("1 KB"));
	size1.Printf(wxT("2 KB"));
	size2.Printf(wxT("4 KB"));
	size3.Printf(wxT("8 KB"));
	size4.Printf(wxT("16 KB"));
	size5.Printf(wxT("32 KB"));
	size6.Printf(wxT("64 KB"));

	if (size0.compare(sampleRamSize->GetValue()) == 0) {
		channelInfo->sampleSizeIndex = 0;
		channelInfo->blockTransferSize = 4;
	}
	else if (size1.compare(sampleRamSize->GetValue()) == 0) {
		channelInfo->sampleSizeIndex = 1;
		channelInfo->blockTransferSize = 8;
	}
	else if (size2.compare(sampleRamSize->GetValue()) == 0) {
		channelInfo->sampleSizeIndex = 2;
		channelInfo->blockTransferSize = 16;
	}
	else if (size3.compare(sampleRamSize->GetValue()) == 0) {
		channelInfo->sampleSizeIndex = 3;
		channelInfo->blockTransferSize = 32;
	}
	else if (size4.compare(sampleRamSize->GetValue()) == 0) {
		channelInfo->sampleSizeIndex = 4;
		channelInfo->blockTransferSize = 64;
	}
	else if (size5.compare(sampleRamSize->GetValue()) == 0) {
		channelInfo->sampleSizeIndex = 5;
		channelInfo->blockTransferSize = 128;
	}
	else {
		channelInfo->sampleSizeIndex = 6;
		channelInfo->blockTransferSize = 256;
	}

	if (port->isConnected()) {
		port->writeCommand(&READY_TO_TX_SAMPLE_SIZE);
		SetStatusText(wxT("ready to tx complete"));
		while (!port->readSerialPort(&receivedWord, 1));

		if (receivedWord == READY_TO_RX) {
			port->writeCommand(&channelInfo->sampleSizeIndex);
			port->readSerialPort(&receivedWord, 1);
			if (receivedWord == ACK_FROM_RX) {
				SetStatusText(wxT("Complete setting sample size!"));
				wave->scrollbar->SetScrollbar(wave->sampleIndex, 5, channelInfo->blockTransferSize * 256, 100);
			}
			else SetStatusText(wxT("Failed to receive ACK from analyzer on select sample size!"));
		}
		else SetStatusText(wxT("Failed to receive ready from analyzer on select sample size!"));
	}
	else SetStatusText(wxT("Connect to serial port!"));
	//wxString msg;
	//msg.Printf(wxT("RAM size: %s, index = %d"), sampleRamSize->GetValue(), channelInfo->sampleSizeIndex);
	//wxMessageBox(msg, wxT("ram size"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::zoomIn(wxCommandEvent& e)
{
	if (wave->zoomIndex < 9) {
		wave->zoomIndex++;
		wave->redrawMainCanvas();
	}
}

void MainFrame::zoomOut(wxCommandEvent& e)
{
	if (wave->zoomIndex > 0) {
		wave->zoomIndex--;
		wave->redrawMainCanvas();
	}
}

void MainFrame::forward(wxCommandEvent& e)
{
	if (wave->sampleIndex < channelInfo->blockTransferSize* 256) {
		wave->sampleIndex += wave->sampleShift[wave->zoomIndex];
		if (wave->sampleIndex > (channelInfo->blockTransferSize * 256) - 7) wave->sampleIndex = (channelInfo->blockTransferSize * 256) - 7;
		SetStatusText(wxString::Format(wxT("index=%d"), wave->sampleIndex));
		wave->scrollbar->SetThumbPosition(wave->sampleIndex);
		wave->redrawMainCanvas();
	}
}

void MainFrame::backward(wxCommandEvent& e)
{
	if (wave->sampleIndex > 0) {
		wave->sampleIndex -= wave->sampleShift[wave->zoomIndex];
		if (wave->sampleIndex < 0) wave->sampleIndex = 0;
		wave->scrollbar->SetThumbPosition(wave->sampleIndex);
		SetStatusText(wxString::Format(wxT("index=%d"), wave->sampleIndex));
		wave->redrawMainCanvas();
	}
}

uint8_t MainFrame::getChannelNumber(wxString channelName)
{
	wxString channel0, channel1, channel2, channel3, channel4, channel5, channel6;
	channel0.Printf("Channel 0");
	channel1.Printf("Channel 1");
	channel2.Printf("Channel 2");
	channel3.Printf("Channel 3");
	channel4.Printf("Channel 4");
	channel5.Printf("Channel 5");
	channel6.Printf("Channel 6");

	//wxMessageBox(channelName, wxT("chanel name"), wxOK | wxICON_INFORMATION, this);
	
	if (channel0.compare(channelName) == 0) return 0;
	else if (channel1.compare(channelName) == 0) return 1;
	else if (channel2.compare(channelName) == 0) return 2;
	else if (channel3.compare(channelName) == 0) return 3;
	else if (channel4.compare(channelName) == 0) return 4;
	else if (channel5.compare(channelName) == 0) return 5;
	else if (channel6.compare(channelName) == 0) return 6;
	else return 7;
}
