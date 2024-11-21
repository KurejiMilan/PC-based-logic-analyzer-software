#include "PortDialog.h"

BEGIN_EVENT_TABLE(ManagePort, wxDialog)
	EVT_BUTTON(SCAN_BUTTON_ID, ManagePort::ScanPorts)
	EVT_BUTTON(CONNECT_BUTTON_ID, ManagePort::connectToCOMPort)
	EVT_UPDATE_UI(CONNECT_BUTTON_ID, ManagePort::UpdateConnectButtonUI)
END_EVENT_TABLE()

ManagePort::ManagePort(wxWindow* parent, wxWindowID id, const wxString& title, SerialPort* srl) : wxDialog(parent, id, title, wxPoint(420, 200))
{
	serial = srl;
	SetMinSize(wxSize(600, 50));
	wxBoxSizer* mainSeizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* managePortIntro = new wxStaticText(this, wxID_ANY, wxT("Connect the Logic Analyzer to PC before scaning for the device and connecting to the device."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	statusText = new wxStaticText(this, wxID_ANY, wxT("Not connected to any Port"));

	wxButton* scanButton = new wxButton(this, SCAN_BUTTON_ID, wxT("Scan PORTs"));
	
	wxArrayString strArray;
	
	
	strArray.Add(wxT("None"));

	strArray.Add(wxT(""));
	strArray.Add(wxT(""));

	mainSeizer->Add(managePortIntro, 0, wxALL, 5);
	mainSeizer->Add(scanButton, 0, wxLEFT|wxBOTTOM, 5);
	
	scanedPorts = new wxComboBox(this, SCANED_PORT_LIST_COMBO_ID, wxT("None"), wxDefaultPosition, wxSize(110, 30), strArray, wxCB_READONLY);
	if (serial->isConnected()) scanedPorts->SetString(0, serial->ConnectedPort);
	if (serial->isConnected()) {
		statusText->SetLabel(wxString::Format(wxT("Connected to %s!"), serial->ConnectedPort));
	}
	mainSeizer->Add(scanedPorts, 0, wxLEFT|wxBOTTOM, 5);

	wxButton* connectCOMPort = new wxButton(this, CONNECT_BUTTON_ID, wxT("Connect"), wxDefaultPosition);
	mainSeizer->Add(connectCOMPort, 0, wxLEFT | wxBOTTOM | wxALIGN_CENTER, 5);

	
	mainSeizer->Add(statusText, 0, wxLEFT, 5);
	SetSizer(mainSeizer);
	Fit();
}


void ManagePort::ScanPorts(wxCommandEvent& event)
{
	// scan from port0 to port 100, only lists the first port it discovers
	for (unsigned int i = 1; i < 101; i++) {
		if (serial->ScanCOMPort(i)) {
			scanedPorts->SetString(totalScanedPorts, wxString::Format(wxT("\\\\.\\COM%d"), i));
			totalScanedPorts++;
		}
		if (totalScanedPorts > 2)break;																			//for now only 3 COM ports can be listed
	}
	if ((totalScanedPorts == 0)&&(!serial->isConnected())) scanedPorts->SetString(0, wxT("None"));
	totalScanedPorts = 0;
}

void ManagePort::connectToCOMPort(wxCommandEvent&)
{
	//only connect to the com Port with which connection is not currently establish 
	//disconnect to with the currently connected port to connect with new serial PORT
	if (serial->ConnectedPort.compare(scanedPorts->GetValue()) != 0) {
		if (serial->disconnect()) {
			wxString portName;
			portName.Printf(scanedPorts->GetValue());
			if (serial->establishConnection(portName)) {
				statusText->SetLabel(wxString::Format(wxT("Connected to %s!"), portName));
			}
			else {
				statusText->SetLabel(wxString::Format(wxT("Failed to connect to %s"), portName));
			}
		}

	}
}

void ManagePort::UpdateConnectButtonUI(wxUpdateUIEvent& event)
{
	wxString strNone;
	wxString eptStr;
	strNone.Printf(wxT("None"));
	eptStr.Printf(wxT(""));
	if (strNone.compare(scanedPorts->GetValue()) == 0) {
		event.Enable(false);
	}
	else if(eptStr.compare(scanedPorts->GetValue()) == 0){
		event.Enable(false);
	}
	else {
		event.Enable(true);
	}
}
