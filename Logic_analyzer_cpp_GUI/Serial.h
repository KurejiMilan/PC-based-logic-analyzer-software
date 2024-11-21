/*
	SOURCES FOR WIN32 APIS
		https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea#syntax
		https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-comstat
		https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-dcb

	Forum for scaning the connected ports
		https://cboard.cprogramming.com/windows-programming/157430-auto-detect-com-ports-using-c.html
*/
#pragma once

//#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include "wx/wx.h"
#include<windows.h>
/*
This is a port from other application 
*/

#include<stdint.h>

class SerialPort {
private:
	HANDLE handler;
	bool connected = false;
	COMSTAT status;
	DWORD errors;
public:
	SerialPort();									//constructor function
	~SerialPort();									//destructor function

	wxString ConnectedPort;							// used to store the connected serial port Name
	bool disconnect();
	bool establishConnection(wxString);

	int readSerialPort(uint8_t*, unsigned int);		// readSerialPort function to read the serial data
	bool writeSerialPort(uint8_t*, unsigned int);	// writeSerialPort function to write a serial data
	bool writeCommand(uint8_t*);						// used to send handshake signals	
	
	bool isConnected();								// isConnected function to check if the connection is establised
	bool ScanCOMPort(uint8_t);
};
