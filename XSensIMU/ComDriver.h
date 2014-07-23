#pragma once
#include <Windows.h>

#define M3D_COMM_FAILED				-1
#define M3D_COMM_INVALID_PORTNUM	-2
#define M3D_COMM_WRITE_ERROR		-3
#define M3D_COMM_READ_ERROR			-4
#define M3D_COMM_RDLEN_ERROR		-5
#define M3D_COMM_RDTIMEOUT_ERROR	-6
#define M3D_CHECKSUM_ERROR			-7
#define MAX_PORT_NUM				20
#define M3D_COMM_NOPARITY			0


#define M3D_COMM_ODDPARITY			1
#define M3D_COMM_EVENPARITY			2
#define M3D_COMM_ONESTOPBIT			1
#define M3D_COMM_TWOSTOPBITS		2
#define M3D_COMM_OK					0	 
#define M3D_OK						1

class ComDriver  
{
public:
	BYTE getChar();
	BYTE receiveBuf[1024];
	BYTE sendBuf[1024];
	void closePort();
	int receiveData(int responseLength);
	int sendData(BYTE* command, int commandLength);
	int setCommTimeouts(int portNum, int readTimeout, int writeTimeout);
	int setCommParameters(int portNum, int baudrate, int charsize, int parity, int stopbits);
	int openPort(int portNum, int baudrate, int size, int parity, int stopbits);
	HANDLE portHandles;
	ComDriver();
	~ComDriver();

};
