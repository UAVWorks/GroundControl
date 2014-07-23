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
		return m_com.openPort(port, baudrate, 8, 0, 1);
	}

	void ComSensor::quit()
	{
		m_com.closePort();
	}

	void ComSensor::registerSensorCallback(ComSensorCallback callback, void* param)
	{
		m_callback = callback;
		m_callbackParam = param;
	}

}