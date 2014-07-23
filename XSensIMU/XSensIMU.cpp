// XSensIMU.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <xsensdeviceapi.h> // The Xsens device API header
#include "serialkey.h"

#include <iostream>
#include <list>
#include <iomanip>
#include <stdexcept>

#include <xsens/xstime.h>

#include "conio.h" // for non ANSI _kbhit() and _getch()
#include "ComDriver.h"

using namespace std;

class CallbackHandler : public XsCallback
{
public:
	CallbackHandler(size_t maxBufferSize = 5) : m_maxNumberOfPacketsInBuffer(maxBufferSize), m_numberOfPacketsInBuffer(0)
#ifdef _MSC_VER
	{
		InitializeCriticalSection(&m_CriticalSection);
	}
	virtual ~CallbackHandler() throw() { DeleteCriticalSection(&m_CriticalSection); }
#else
	{
		//create mutex attribute variable
		pthread_mutexattr_t mAttr;

		// setup recursive mutex for mutex attribute
		pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);

		// Use the mutex attribute to create the mutex
		pthread_mutex_init(&m_CriticalSection, &mAttr);

		// Mutex attribute can be destroy after initializing the mutex variable
		pthread_mutexattr_destroy(&mAttr);

	}
	virtual ~CallbackHandler() throw() { pthread_mutex_destroy(&m_CriticalSection); }
#endif

	bool packetAvailable() const { Locker lock(*this); return m_numberOfPacketsInBuffer > 0; }
	XsDataPacket getNextPacket()
	{
		assert(packetAvailable());
		Locker lock(*this);
		XsDataPacket oldestPacket(m_packetBuffer.front());
		m_packetBuffer.pop_front();
		--m_numberOfPacketsInBuffer;
		return oldestPacket;
	}

protected:
	virtual void onDataAvailable(XsDevice*, const XsDataPacket* packet)
	{
		Locker lock(*this);
		assert(packet != 0);
		while (m_numberOfPacketsInBuffer >= m_maxNumberOfPacketsInBuffer)
		{
			(void)getNextPacket();
		}
		m_packetBuffer.push_back(*packet);
		++m_numberOfPacketsInBuffer;
		assert(m_numberOfPacketsInBuffer <= m_maxNumberOfPacketsInBuffer);
	}
private:
#ifdef _MSC_VER
	mutable CRITICAL_SECTION m_CriticalSection;
#else
	mutable pthread_mutex_t m_CriticalSection;
#endif
	struct Locker
	{
#ifdef _MSC_VER
		Locker(CallbackHandler const & self) : m_self(self) { EnterCriticalSection(&m_self.m_CriticalSection); }
		~Locker() throw() { LeaveCriticalSection(&m_self.m_CriticalSection); }
#else
		Locker(CallbackHandler const & self) : m_self(self) { pthread_mutex_lock(&m_self.m_CriticalSection); }
		~Locker() throw() { pthread_mutex_unlock(&m_self.m_CriticalSection); }
#endif
		CallbackHandler const & m_self;
	};
	size_t m_maxNumberOfPacketsInBuffer;
	size_t m_numberOfPacketsInBuffer;
	std::list<XsDataPacket> m_packetBuffer;
};



int _tmain(int argc, _TCHAR* argv[])
{
	if (!setSerialKey())
	{
		std::cout << "Invalid serial key." << std::endl;
		std::cout << "Press [ENTER] to continue." << std::endl; std::cin.get();
		return 1;
	}


	ComDriver com;
	com.openPort(4, 115200, 8, 0, 1);


	// Create XsControl object
	std::cout << "Creating XsControl object..." << std::endl;
	XsControl* control = XsControl::construct();
	assert(control != 0);

	try
	{
		// Scan for connected devices
		std::cout << "Scanning for devices..." << std::endl;
		XsPortInfoArray portInfoArray = XsScanner::scanPorts();

		// Find an MTi / MTx / MTmk4 device
		XsPortInfoArray::const_iterator mtPort = portInfoArray.begin();
		while (mtPort != portInfoArray.end() && !mtPort->deviceId().isMtix() && !mtPort->deviceId().isMtMk4()) { ++mtPort; }
		if (mtPort == portInfoArray.end())
		{
			throw std::runtime_error("No MTi / MTx / MTmk4 device found. Aborting.");
		}
		std::cout << "Found a device with id: " << mtPort->deviceId().toString().toStdString() << " @ port: " << mtPort->portName().toStdString() << ", baudrate: " << mtPort->baudrate() << std::endl;

		// Open the port with the detected device
		std::cout << "Opening port..." << std::endl;
		if (!control->openPort(mtPort->portName().toStdString(), mtPort->baudrate()))
		{
			throw std::runtime_error("Could not open port. Aborting.");
		}

		try
		{
			// Get the device object
			XsDevice* device = control->device(mtPort->deviceId());
			assert(device != 0);

			// Print information about detected MTi / MTx / MTmk4 device
			std::cout << "Device: " << device->productCode().toStdString() << " opened." << std::endl;

			// Create and attach callback handler to device
			CallbackHandler callback;
			device->addCallbackHandler(&callback);

			// Put the device in configuration mode
			std::cout << "Putting device into configuration mode..." << std::endl;
			if (!device->gotoConfig()) // Put the device into configuration mode before configuring the device
			{
				throw std::runtime_error("Could not put device into configuration mode. Aborting.");
			}

			// Configure the device. Note the differences between MTix and MTmk4
			std::cout << "Configuring the device..." << std::endl;
			if (device->deviceId().isMtix())
			{
				XsOutputMode outputMode = XOM_Orientation; // output orientation data
				XsOutputSettings outputSettings = XOS_OrientationMode_Quaternion; // output orientation data as quaternion
				XsDeviceMode deviceMode(100); // make a device mode with update rate: 100 Hz
				deviceMode.setModeFlag(outputMode);
				deviceMode.setSettingsFlag(outputSettings);

				// set the device configuration
				if (!device->setDeviceMode(deviceMode))
				{
					throw std::runtime_error("Could not configure MTmki device. Aborting.");
				}
			}
			else if (device->deviceId().isMtMk4())
			{
				XsOutputConfiguration quat(XDI_Quaternion, 30);
				XsOutputConfigurationArray configArray;
				configArray.push_back(quat);
				if (!device->setOutputConfiguration(configArray))
				{

					throw std::runtime_error("Could not configure MTmk4 device. Aborting.");
				}
			}
			else
			{
				throw std::runtime_error("Unknown device while configuring. Aborting.");
			}

			// Put the device in measurement mode
			std::cout << "Putting device into measurement mode..." << std::endl;
			if (!device->gotoMeasurement())
			{
				throw std::runtime_error("Could not put device into measurement mode. Aborting.");
			}

			std::cout << "\nMain loop (press any key to quit)" << std::endl;
			std::cout << std::string(79, '-') << std::endl;
			while (!_kbhit())
			{
				if (callback.packetAvailable())
				{
					// Retrieve a packet
					XsDataPacket packet = callback.getNextPacket();

					// Get the quaternion data
					XsQuaternion quaternion = packet.orientationQuaternion();
					std::cout << "\r"
						<< "q0:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_w
						<< ",q1:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_x
						<< ",q2:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_y
						<< ",q3:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_z
						;

					char str[100] ;
					sprintf_s(str, "\r\n*%.4f,%.4f,%.4f,%.4f", quaternion.m_w, quaternion.m_x, quaternion.m_y, quaternion.m_z);
					int size = strnlen_s(str, 100);
					int sendsize = com.sendData((unsigned char*)str, size);

					// Convert packet to euler
					XsEuler euler = packet.orientationEuler();
					std::cout << ",Roll:" << std::setw(7) << std::fixed << std::setprecision(2) << euler.m_roll
						<< ",Pitch:" << std::setw(7) << std::fixed << std::setprecision(2) << euler.m_pitch
						<< ",Yaw:" << std::setw(7) << std::fixed << std::setprecision(2) << euler.m_yaw
						;

					std::cout << std::flush;
				}
				XsTime::msleep(0);
			}
			_getch();
			std::cout << "\n" << std::string(79, '-') << "\n";
			std::cout << std::endl;
		}
		catch (std::runtime_error const & error)
		{
			std::cout << error.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown fatal error has occured. Aborting." << std::endl;
		}

		// Close port
		std::cout << "Closing port..." << std::endl;
		control->closePort(mtPort->portName().toStdString());
	}
	catch (std::runtime_error const & error)
	{
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "An unknown fatal error has occured. Aborting." << std::endl;
	}

	// Free XsControl object
	std::cout << "Freeing XsControl object..." << std::endl;
	control->destruct();

	std::cout << "Successful exit." << std::endl;

	std::cout << "Press [ENTER] to continue." << std::endl; std::cin.get();



	return 0;
}

