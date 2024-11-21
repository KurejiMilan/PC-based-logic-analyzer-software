#include "DecodeDialog.h"

BEGIN_EVENT_TABLE(DecodeDialog, wxDialog)
	EVT_COMBOBOX(PROTOCOL_LIST_ID, DecodeDialog::updateDecodeConfig)
END_EVENT_TABLE()

DecodeDialog::DecodeDialog(wxWindow* parent, wxWindowID id, const wxString& title, Channel* cPtr) : wxDialog(parent, id, title, wxPoint(420, 200))
{
	SetMinSize(wxSize(500, 200));
	channelPtr = cPtr;
	wxArrayString channelList;
	channelList.Add("Channel 0");
	channelList.Add("Channel 1");
	channelList.Add("Channel 2");
	channelList.Add("Channel 3");
	channelList.Add("Channel 4");
	channelList.Add("Channel 5");
	channelList.Add("Channel 6");
	channelList.Add("Channel 7");

	wxArrayString enabledChannelList;
	enabledChannelList.Add(wxT("--------"));
	for (uint8_t i = 0; i < 8; i++) {
		if (channelPtr->active8ChannelMask & (0x01 << i)) enabledChannelList.Add(wxString::Format(wxT("Channel %d"), i));
	}
	wxArrayString protocolList;
	protocolList.Add("None");
	protocolList.Add("I2C");
	protocolList.Add("SPI");
	protocolList.Add("UART");
	protocolList.Add("Parallel");

	wxBoxSizer* mainSeizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* info = new wxStaticText(this, wxID_ANY, wxT("Select the protocol to be decoded"));
	mainSeizer->Add(info, 0, wxTOP | wxLEFT, 5);
	protocolListComboBox = new wxComboBox(this, PROTOCOL_LIST_ID, wxT("None"), wxDefaultPosition, wxSize(110, 30), protocolList, wxCB_READONLY);
	mainSeizer->Add(protocolListComboBox, 0, wxTOP|wxLEFT, 5);

	i2c = new wxPanel(this, wxID_ANY);
	spi = new wxPanel(this, wxID_ANY);
	uart = new wxPanel(this, wxID_ANY);
	parallel = new wxPanel(this, wxID_ANY);
	

	mainSeizer->Add(i2c, 1, wxEXPAND | wxLEFT | wxBOTTOM, 5);
	mainSeizer->Add(spi, 1, wxEXPAND | wxLEFT | wxBOTTOM, 5);
	mainSeizer->Add(uart, 1, wxEXPAND | wxLEFT | wxBOTTOM, 5);
	mainSeizer->Add(parallel, 1, wxEXPAND | wxLEFT | wxBOTTOM, 5);

	/*
		This section is for I2C decode option
	*/
	// defining sizer for I2C decode option
	wxBoxSizer* I2Cmainsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* I2CSda = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* I2CScl = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* I2Ctext = new wxStaticText(i2c, wxID_ANY, wxT("Select the I2C SDA and SCL lines."));
	I2Cmainsizer->Add(I2Ctext, 0, wxLEFT | wxTOP, 5);

	wxStaticText* I2Csdatext = new wxStaticText(i2c, wxID_ANY, wxT("SDA line"));
	I2CSda->Add(I2Csdatext, 1, wxTOP| wxLEFT |wxBOTTOM, 5);
	I2CSdaCombo = new wxComboBox(i2c, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	I2CSda->Add(I2CSdaCombo, 1, wxDOWN, 5);
	
	wxStaticText* I2Cscltext = new wxStaticText(i2c, wxID_ANY, wxT("SCL line"));
	I2CScl->Add(I2Cscltext, 1, wxTOP | wxLEFT | wxBOTTOM, 5);
	I2CSclCombo = new wxComboBox(i2c, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	I2CScl->Add(I2CSclCombo, 1, wxDOWN, 5);
	I2Cmainsizer->AddSpacer(8);
	I2Cmainsizer->Add(I2CSda);
	I2Cmainsizer->Add(I2CScl);
	i2c->SetSizer(I2Cmainsizer);
	i2c->Fit();
	/*This section is for SPI decode section*/
	wxBoxSizer* SPImainsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* SPICs = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* SPISck = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* SPIMosi = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* SPIMiso = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* SPIinfotext = new wxStaticText(spi, wxID_ANY, wxT("Select the SPI CS, SCK, MISO AND MOSI lines."));
	SPImainsizer->Add(SPIinfotext, 0, wxLEFT | wxTOP, 5);
	
	wxStaticText* SPICsText = new wxStaticText(spi, wxID_ANY, wxT("CS"));
	SPICs->Add(SPICsText, 1, wxTOP | wxLEFT | wxBOTTOM, 5);
	SPICsCombo = new wxComboBox(spi, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	SPICs->Add(SPICsCombo, 1, wxDOWN, 5);

	wxStaticText* SPISckText = new wxStaticText(spi, wxID_ANY, wxT("SCK"));
	SPISck->Add(SPISckText, 1, wxTOP | wxLEFT | wxBOTTOM, 5);
	SPISckCombo = new wxComboBox(spi, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	SPISck->Add(SPISckCombo, 1, wxDOWN, 5);

	wxStaticText* SPIMosiText = new wxStaticText(spi, wxID_ANY, wxT("MOSI"));
	SPIMosi->Add(SPIMosiText, 1, wxTOP | wxLEFT | wxBOTTOM, 5);
	SPIMosiCombo = new wxComboBox(spi, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	SPIMosi->Add(SPIMosiCombo, 1, wxDOWN, 5);

	wxStaticText* SPIMisoText = new wxStaticText(spi, wxID_ANY, wxT("MISO"));
	SPIMiso->Add(SPIMisoText, 1, wxTOP | wxLEFT | wxBOTTOM, 5);
	SPIMisoCombo = new wxComboBox(spi, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	SPIMiso->Add(SPIMisoCombo, 1, wxDOWN, 5);

	SPImainsizer->AddSpacer(8);
	SPImainsizer->Add(SPICs);
	SPImainsizer->Add(SPISck);
	SPImainsizer->Add(SPIMosi);
	SPImainsizer->Add(SPIMiso);
	spi->SetSizer(SPImainsizer);
	spi->Fit();

	/*
		This section is for UART decode option
	*/
	//	defining sizers for UART decode option
	wxBoxSizer* UARTmainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* UARTTx = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* UARTRx = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* UARTSpeed = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* UARTDataFrame = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* UARTParity = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* UARTStopBit = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* UARTInfo = new wxStaticText(uart, wxID_ANY, wxT("Select the UART Tx and Rx Lines"));
	UARTmainSizer->Add(UARTInfo, 0, wxLEFT | wxTOP, 5);

	wxStaticText* UARTTxText = new wxStaticText(uart , wxID_ANY, wxT("Transmit(Tx)"));
	UARTTx->Add(UARTTxText, 1, wxLEFT|wxTOP|wxBOTTOM, 5);
	UARTTxCombo = new wxComboBox(uart, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	UARTTx->Add(UARTTxCombo, 1,  wxBOTTOM, 5);
	UARTmainSizer->Add(UARTTx);
	
	wxStaticText* UARTRxText = new wxStaticText(uart, wxID_ANY, wxT("Receive(Rx)"));
	UARTRx->Add(UARTRxText, 1, wxLEFT | wxTOP|wxBOTTOM, 5);
	UARTRxCombo = new wxComboBox(uart, wxID_ANY, wxT("--------"), wxDefaultPosition, wxSize(110, 25), enabledChannelList, wxCB_READONLY);
	UARTRx->Add(UARTRxCombo, 1,  wxBOTTOM, 5);
	UARTmainSizer->Add(UARTRx);
	
	UARTmainSizer->AddSpacer(15);
	wxStaticText* UARTparameterTexts = new wxStaticText(uart, wxID_ANY, wxT("Set the correct UART parameter"));
	UARTmainSizer->Add(UARTparameterTexts, 0, wxLEFT | wxTOP, 5);

	//9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1000000, 1500000
	wxArrayString UartBuadArray;
	UartBuadArray.Add(wxT("9600"));
	UartBuadArray.Add(wxT("19200"));
	UartBuadArray.Add(wxT("38400"));
	UartBuadArray.Add(wxT("57600"));
	UartBuadArray.Add(wxT("115200"));
	UartBuadArray.Add(wxT("230400"));
	UartBuadArray.Add(wxT("460800"));
	UartBuadArray.Add(wxT("921600"));
	UartBuadArray.Add(wxT("1000000"));
	UartBuadArray.Add(wxT("1500000"));

	wxStaticText* UARTSpeedText = new wxStaticText(uart, wxID_ANY, wxT("BAUD Rate"));
	UARTSpeed->Add(UARTSpeedText, 1, wxLEFT|wxTOP|wxBOTTOM, 5);
	UARTBuadCombo = new wxComboBox(uart, wxID_ANY, wxT("9600"), wxDefaultPosition, wxSize(110, 25), UartBuadArray, wxCB_READONLY);
	UARTSpeed->Add(UARTBuadCombo, 1, wxBOTTOM, 5);
	UARTmainSizer->Add(UARTSpeed);

	wxArrayString FramebitsArray;
	FramebitsArray.Add(wxT("5"));
	FramebitsArray.Add(wxT("6"));
	FramebitsArray.Add(wxT("7"));
	FramebitsArray.Add(wxT("8"));
	FramebitsArray.Add(wxT("9"));
	
	wxStaticText* UARTDataBits = new wxStaticText(uart, wxID_ANY, wxT("Frame bits"));
	UARTDataFrame->Add(UARTDataBits, 1, wxLEFT | wxTOP | wxBOTTOM, 5);
	UARTFrameBitsCombo = new wxComboBox(uart, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize(110, 25), FramebitsArray, wxCB_READONLY);
	UARTDataFrame->Add(UARTFrameBitsCombo, 1, wxBOTTOM, 5);
	UARTmainSizer->Add(UARTDataFrame);
	
	wxArrayString parityBitArray;
	parityBitArray.Add(wxT("None"));
	parityBitArray.Add(wxT("Odd"));
	parityBitArray.Add(wxT("Even"));

	wxStaticText* UARTParityText = new wxStaticText(uart, wxID_ANY, wxT("Parity bit"));
	UARTParity->Add(UARTParityText, 1, wxLEFT | wxTOP | wxBOTTOM, 5);
	UARTParityCombo = new wxComboBox(uart, wxID_ANY, wxT("None"), wxDefaultPosition, wxSize(110, 25), parityBitArray, wxCB_READONLY);
	UARTParity->Add(UARTParityCombo, 1, wxBOTTOM, 5);
	UARTmainSizer->Add(UARTParity);

	wxArrayString stopBitsArray;
	stopBitsArray.Add(wxT("1"));
	stopBitsArray.Add(wxT("2"));
	wxStaticText* UARTStopBitsText = new wxStaticText(uart, wxID_ANY, wxT("Stop bits"));
	UARTStopBit->Add(UARTStopBitsText, 1, wxLEFT | wxTOP | wxBOTTOM, 5);
	UARTStopBitsCombo = new wxComboBox(uart, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(110, 25), stopBitsArray, wxCB_READONLY);
	UARTStopBit->Add(UARTStopBitsCombo, 1, wxBOTTOM, 5);
	UARTmainSizer->Add(UARTStopBit);

	uart->SetSizer(UARTmainSizer);
	uart->Fit();
	/*
		This section is for parallel decode option
	*/
	// defining sizers for the parallel panel
	wxBoxSizer* parallelMainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* dataline1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline5 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline6 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline7 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* dataline8 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* dataline1text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 1"));
	dataline1->Add(dataline1text, 1, wxLEFT|wxBOTTOM|wxTOP, 5);
	dataline1Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL1_ID, wxT("Channel 0"), wxDefaultPosition, wxSize(110, 25),channelList, wxCB_READONLY);
	dataline1->Add(dataline1Combo, 0, wxBOTTOM|wxLEFT, 5);

	wxStaticText* dataline2text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 2"));
	dataline2->Add(dataline2text, 1, wxLEFT | wxBOTTOM | wxTOP, 5);
	dataline2Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL2_ID, wxT("Channel 1"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline2->Add(dataline2Combo, 0, wxBOTTOM | wxLEFT, 5);

	wxStaticText* dataline3text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 3"));
	dataline3->Add(dataline3text, 1, wxLEFT | wxBOTTOM |wxTOP, 5);
	dataline3Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL3_ID, wxT("Channel 2"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline3->Add(dataline3Combo, 0, wxBOTTOM | wxLEFT, 5);

	wxStaticText* dataline4text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 4"));
	dataline4->Add(dataline4text, 1, wxLEFT | wxBOTTOM | wxTOP, 5);
	dataline4Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL4_ID, wxT("Channel 3"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline4->Add(dataline4Combo, 0, wxBOTTOM | wxLEFT, 5);

	wxStaticText* dataline5text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 5"));
	dataline5->Add(dataline5text, 1, wxLEFT | wxBOTTOM | wxTOP, 5);
	dataline5Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL5_ID, wxT("Channel 4"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline5->Add(dataline5Combo, 0, wxBOTTOM | wxLEFT, 5);

	wxStaticText* dataline6text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 6"));
	dataline6->Add(dataline6text, 1, wxLEFT | wxBOTTOM | wxTOP, 5);
	dataline6Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL6_ID, wxT("Channel 5"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline6->Add(dataline6Combo, 0, wxBOTTOM | wxLEFT, 5);

	wxStaticText* dataline7text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 7"));
	dataline7->Add(dataline7text, 1, wxLEFT | wxBOTTOM | wxTOP, 5);
	dataline7Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL7_ID, wxT("Channel 6"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline7->Add(dataline7Combo, 0, wxBOTTOM | wxLEFT, 5);

	wxStaticText* dataline8text = new wxStaticText(parallel, wxID_ANY, wxT("DataLine 8"));
	dataline8->Add(dataline8text, 1, wxLEFT | wxBOTTOM | wxTOP, 5);
	dataline8Combo = new wxComboBox(parallel, PARALLEL_COMBO_CHANNEL8_ID, wxT("Channel 7"), wxDefaultPosition, wxSize(110, 25), channelList, wxCB_READONLY);
	dataline8->Add(dataline8Combo, 0, wxBOTTOM | wxLEFT, 5);

	// disabling which channel to be set as which dataline for now
	// to enbale this further code needs to be implemented could be added later updates
	dataline1Combo->Enable(false);
	dataline2Combo->Enable(false);
	dataline3Combo->Enable(false);
	dataline4Combo->Enable(false);
	dataline5Combo->Enable(false);
	dataline6Combo->Enable(false);
	dataline7Combo->Enable(false);
	dataline8Combo->Enable(false);

	parallelMainSizer->Add(dataline1);
	parallelMainSizer->Add(dataline2);
	parallelMainSizer->Add(dataline3);
	parallelMainSizer->Add(dataline4);
	parallelMainSizer->Add(dataline5);
	parallelMainSizer->Add(dataline6);
	parallelMainSizer->Add(dataline7);
	parallelMainSizer->Add(dataline8);

	parallel->SetSizer(parallelMainSizer);
	parallel->Fit();
	
	i2c->Hide();
	spi->Hide();
	uart->Hide();
	parallel->Hide();

	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	wxButton* okButton = new wxButton(this, wxID_OK, wxT("Okay"));
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->AddSpacer(120);
	buttonSizer->Add(cancelButton,1, wxALIGN_BOTTOM|wxBottom, 10);
	buttonSizer->AddSpacer(10);
	buttonSizer->Add(okButton,1, wxALIGN_BOTTOM | wxBottom, 10);
	mainSeizer->Add(buttonSizer);

	SetSizer(mainSeizer);
	Fit();
}

/*
	UI update event to show different decode option based on the selected value
*/

void DecodeDialog::updateDecodeConfig(wxCommandEvent&)
{
	wxString i2ct, spit, uartt, parallelt;
	i2ct.Printf("I2C");
	spit.Printf("SPI");
	uartt.Printf("UART");
	parallelt.Printf("Parallel");

	i2c->Hide();
	spi->Hide();
	uart->Hide();
	parallel->Hide();

	if (i2ct.compare(protocolListComboBox->GetValue()) == 0) i2c->Show();
	else if(spit.compare(protocolListComboBox->GetValue()) == 0) spi->Show();
	else if (uartt.compare(protocolListComboBox->GetValue()) == 0) uart->Show();
	else if (parallelt.compare(protocolListComboBox->GetValue()) == 0) parallel->Show();

	Layout();
	Fit();
}
