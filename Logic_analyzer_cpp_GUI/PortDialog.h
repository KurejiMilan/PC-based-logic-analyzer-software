#pragma once
#include "wx/wx.h"
#include "Serial.h"

enum {
	SCAN_BUTTON_ID = 400,
	SCANED_PORT_LIST_COMBO_ID = 401,
	CONNECT_BUTTON_ID = 402
};

class ManagePort: public wxDialog {
public:
	ManagePort(wxWindow*, wxWindowID,const wxString&, SerialPort*);
	void ScanPorts(wxCommandEvent&);
	void connectToCOMPort(wxCommandEvent&);
	void UpdateConnectButtonUI(wxUpdateUIEvent&);

private:
	SerialPort* serial = nullptr;
	wxComboBox* scanedPorts = nullptr;
	wxStaticText* statusText = nullptr;
	unsigned int totalScanedPorts = 0;
	DECLARE_EVENT_TABLE()
};