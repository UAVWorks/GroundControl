// COMM.cpp: implementation of the CCOMM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include "ComDriver.h"




using namespace std;

FILE *fp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ComDriver::ComDriver()
{

}

ComDriver::~ComDriver()
{

}

int ComDriver::openPort(int portNum, int baudrate, int size, int parity, int stopbits)
{	
	int errcheck;
	int porth;

	/* open port */
	HANDLE portHandle;
	char portName[20] = "\\\\.\\COM";
	char portNumber[3];
	_itoa_s(portNum, portNumber, 10);
	strcat_s(portName, portNumber ); 

	string temp = portName;
	wstring port = wstring(temp.begin(), temp.end());

	portHandle = CreateFile(port.c_str(), 
		GENERIC_READ | GENERIC_WRITE,
		0, // exclusive access
		NULL, // no security
		OPEN_EXISTING,
		0, // no overlapped I/O
		NULL); // null template		

	/* set communications parameters */	



	if (portHandle == INVALID_HANDLE_VALUE) errcheck = M3D_COMM_FAILED;		
	else {		
		int res = SetupComm(portHandle, 1024, 1024);	// set buffer sizes
		if(res == 0)
		{
			int err = GetLastError();
		}
		portHandles = portHandle;
		errcheck = portNum;
	}

	if (errcheck<0) {
		return errcheck;
	}
	porth = errcheck;  /* no error, so this is the port number. */

	errcheck = setCommParameters(porth, baudrate, size, parity, stopbits);
	if (errcheck!=M3D_COMM_OK) {
		return errcheck;
	}


	/* set timeouts */	
	errcheck = setCommTimeouts(porth, 200, 200);
	if (errcheck!=M3D_COMM_OK) {
		return errcheck;
	}

	//fp = fopen("log.dat","w+");
	//fflush(fp);
	return porth;

}

int ComDriver::setCommParameters(int portNum, int baudrate, int charsize, int parity, int stopbits)
{
	BOOL ready;
	DCB  dcb;
	HANDLE portHandle;
	portHandle = portHandles;

	ready = GetCommState(portHandle, &dcb);
	if (!ready) {
		int err = GetLastError();
		return M3D_COMM_FAILED;
	}
	if (stopbits==1) {
		stopbits=0;    
	}
	dcb.BaudRate = baudrate;
	dcb.ByteSize = charsize;
	dcb.Parity = parity;
	dcb.StopBits = stopbits;
	dcb.fAbortOnError = TRUE;
	ready = SetCommState(portHandle, &dcb);
	if (!ready)
		return M3D_COMM_FAILED;
	else
		return M3D_COMM_OK;
}

int ComDriver::setCommTimeouts(int portNum, int readTimeout, int writeTimeout)
{
	BOOL ready;
	COMMTIMEOUTS timeOuts;
	HANDLE portHandle;
	portHandle = portHandles;

	ready = GetCommTimeouts (portHandle, &timeOuts);
	if (!ready) {
		return M3D_COMM_FAILED;
	}
	timeOuts.ReadIntervalTimeout = readTimeout;
	timeOuts.ReadTotalTimeoutConstant = readTimeout;
	timeOuts.ReadTotalTimeoutMultiplier = 10;
	timeOuts.WriteTotalTimeoutConstant = writeTimeout;
	timeOuts.WriteTotalTimeoutMultiplier = 10;
	ready = SetCommTimeouts (portHandle, &timeOuts);
	if (!ready) {
		return M3D_COMM_FAILED;
	}
	else
		return M3D_COMM_OK;
}


int ComDriver::sendData(BYTE *command, int commandLength)
{
	DWORD bytesWritten;
	BOOL status;

	HANDLE portHandle;
	portHandle = portHandles;	

	/* write the command. */
	status = WriteFile (portHandle, command, commandLength, &bytesWritten, 0);
	if (!status)
		return M3D_COMM_WRITE_ERROR;  //!! check bytes written?
	else
		return M3D_COMM_OK;
}

int ComDriver::receiveData(int responseLength)
{
	DWORD bytesRead;
	BOOL status;
	int returnVal = M3D_COMM_OK;

	/* receive response data from the serial port if expected. */
	if (responseLength>0) {		

		status = ReadFile(portHandles, receiveBuf, responseLength, &bytesRead, NULL);				
		//	fflush(stdin);
		//	fprintf(fp,"receive data num is %d\n",(int)bytesRead);
		//	for(int i=0;i<(int)bytesRead;i++) fprintf(fp,"%dth data is %02x\n",i,receiveBuf[i]);
		if (status) {
			/* check for wrong number of bytes returned. */
			if (bytesRead == responseLength)
				returnVal = M3D_COMM_OK;
			else
				returnVal = M3D_COMM_RDLEN_ERROR;

		} else
			returnVal = M3D_COMM_READ_ERROR;
	}

	return returnVal;

}

void ComDriver::closePort()
{
	CloseHandle(portHandles);
}

BYTE ComDriver::getChar()
{
	DWORD bytesRead;
	BOOL status;
	BYTE returnVal;

	/* receive response data from the serial port if expected. */		
	status = ReadFile(portHandles, &returnVal, 1, &bytesRead, NULL);	
	//fprintf(fp,"get data is %02x\n",returnVal);
	if(status) return returnVal;
	else return -1;

}
