#include "Serial.h"

SerialPort::SerialPort(){
	ConnectedPort.Printf(wxT(""));
}

SerialPort::~SerialPort()
{
	if (this->connected)
	{
		this->connected = false;
		if (CloseHandle(this->handler) != 0)
		{
			//printf("Handle closed successfully\n");
		}
		else
		{
			//printf("Error in closing object handle handle\n");
		}
	}
}

bool SerialPort::establishConnection(wxString portName)
{
	this->handler = CreateFileA(static_cast<LPCSTR>(portName.mb_str()),//port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices				
		//this is used to open a serial port if succesfull it returns a handler which is used to refer to connection
	if (this->handler == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			wxMessageBox(wxT("ERROR: Handle was not attached.Reason : port not available\n"), wxT("Port not available"), wxOK | wxICON_ERROR);
		}
		else if (GetLastError() == ERROR_SHARING_VIOLATION)
		{
			wxMessageBox(wxT("ERROR: The device or file is already in use by other process\n"), wxT("Port Busy"), wxOK | wxICON_INFORMATION);
		}
	}
	else
	{
		DCB dcbSerialPrameters;		//data control block structure that is used to store configuration for serial deive
		COMMTIMEOUTS timeOutParameters;
		if (GetCommState(this->handler, &dcbSerialPrameters) == 0)
		{
			wxMessageBox(wxT("Failed to get current serial parameters"), wxT("get DCB fail"), wxOK | wxICON_ERROR);
		}
		else
		{
			timeOutParameters.ReadIntervalTimeout = 0;
			timeOutParameters.ReadTotalTimeoutConstant = 0;
			timeOutParameters.ReadTotalTimeoutMultiplier = 0;
			timeOutParameters.WriteTotalTimeoutConstant = 0;
			timeOutParameters.WriteTotalTimeoutMultiplier = 0;//*/

			dcbSerialPrameters.BaudRate = CBR_115200;
			dcbSerialPrameters.ByteSize = 8;
			dcbSerialPrameters.StopBits = ONESTOPBIT;
			dcbSerialPrameters.Parity = NOPARITY;
			dcbSerialPrameters.fDtrControl = DTR_CONTROL_DISABLE;
			dcbSerialPrameters.fRtsControl = RTS_CONTROL_DISABLE;
			if (!SetCommState(this->handler, &dcbSerialPrameters))
			{
				//SetCommState function is used to configure the device according the dcb structure
				//takes two argument the handler retured by CreateFileA and the pointer to the dcb struct data
				//return non zero for succes else 0
				wxMessageBox(wxT("ALERT: could not set Serial port parameters"), wxT("setting port failed"), wxOK | wxICON_ERROR);
			}
			else
			{	
				// disabling timeout
				SetCommTimeouts(this->handler, &timeOutParameters);		
				// connection has established now purgecomm function is used to discrad all the characters in the
				// input and output buffer of the specified communication resource. it can also terminate the 
				// pending read and write operation on the specified resource
				// other alternative is FlushFileBuffers()
				this->connected = true;
				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				ConnectedPort.Printf(portName);
				return true;
				//printf("successfully opened port %s\n", portName);
			}
		}

	}
	return false;
}

bool SerialPort::disconnect()
{
	if (this->connected)
	{
		this->connected = false;
		if (CloseHandle(this->handler) != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else return true;
}

int SerialPort::readSerialPort(uint8_t* buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	//unsigned int toRead = 0;
	ClearCommError(this->handler, &this->errors, &this->status);
	/*if (this->status.cbInQue > 0)
	{
		if (this->status.cbInQue > buf_size)
		{
			toRead = buf_size;
		}
		else toRead = this->status.cbInQue;
	}*/
	if (ReadFile(this->handler, buffer, buf_size, &bytesRead, NULL)){
		//wxMessageBox(wxString::Format(wxT("received size=%d"), bytesRead), wxT("Inside send"), wxOK | wxICON_INFORMATION);
		PurgeComm(this->handler, PURGE_RXCLEAR);
		return bytesRead;
	}

	return 0;
}

bool SerialPort::writeSerialPort(uint8_t* buffer, unsigned int buf_size)
{
	DWORD bytesSend;
	//PurgeComm(this->handler, PURGE_TXCLEAR);
	//printf("im here");
	ClearCommError(this->handler, &this->errors, &this->status);
	if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, NULL))
	{
		ClearCommError(this->handler, &this->errors, &this->status);
		return false;
	}
	else
	{
		//std::cout << "number of bytes transmitted is=" << bytesSend << std::endl;
		//printf("transmitted data is=%c\n", command);
		//std::cout << "number of bytes remaining to be transmitted is=" << this->status.cbOutQue << std::endl;
		return true;
	}
}
bool SerialPort::writeCommand(uint8_t* command)
{
	DWORD bytesSent;
	//std::cout << "number of bytes remaining to be transmitted is=" << this->status.cbOutQue;
	ClearCommError(this->handler, &this->errors, &this->status);
	if (!WriteFile(this->handler, command, 1, &bytesSent, NULL))
	{
		// return false since failed to transmit data
		//printf("error in file write");
		ClearCommError(this->handler, &this->errors, &this->status);
		return false;
	}
	else
	{
		return true;
	}
}

bool SerialPort::isConnected()
{
	return this->connected;
}

bool SerialPort::ScanCOMPort(uint8_t PortNum)
{
	wxString portName;
	portName.Printf(wxT("\\\\.\\COM%d"), PortNum);
	HANDLE tempHandler;
	tempHandler = CreateFileA(static_cast<LPCSTR>(portName.mb_str()),//port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices				
	//this is used to open a serial port if succesfull it returns a handler which is used to refer to connection
	
	if (tempHandler == INVALID_HANDLE_VALUE) return false;
	else {
		CloseHandle(tempHandler);
		return true;
	}

	return false;
}
