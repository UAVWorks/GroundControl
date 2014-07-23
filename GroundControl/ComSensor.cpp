#include "stdafx.h"
#include "ComSensor.h"

namespace GroundControl
{


	ComSensor::ComSensor() : m_callback(NULL), m_callbackParam(NULL)
	{
	}


	ComSensor::~ComSensor()
	{
	}


	int ComSensor::init(int port, int baudrate)
	{
		return m_com.openPort(port, baudrate, 8, 0, 1); // open port 
	}

	void ComSensor::quit()
	{
		m_com.closePort(); // close com port
	}

	void ComSensor::registerSensorCallback(ComSensorCallback callback, void* param)
	{
		m_callback = callback;
		m_callbackParam = param;
	}

}