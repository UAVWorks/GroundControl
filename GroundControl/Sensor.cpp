#include "stdafx.h"
#include "Sensor.h"

namespace GroundControl
{
	Sensor::Sensor(const string& name, DataSource* datasrc) : Element(name), m_dataSrc(datasrc)
	{
	}


	Sensor::~Sensor()
	{
	}

	const float* Sensor::read() const
	{
		return m_data;
	}

	PosSensor::PosSensor(const string& name, DataSource* datasrc) : Sensor(name, datasrc)
	{

	}

	PosSensor::~PosSensor()
	{

	}

	void PosSensor::init()
	{
		m_data = m_posData;
	}

	void PosSensor::update()
	{
		m_dataSrc->getPos(name(), m_posData);
	}


	RotationSensor::RotationSensor(const string& name, DataSource* datasrc) : Sensor(name, datasrc)
	{

	}

	RotationSensor::~RotationSensor()
	{

	}

	void RotationSensor::init()
	{
		m_data = m_rotation;
	}

	void RotationSensor::update()
	{
		m_dataSrc->getRot(name(), m_rotation);
	}


	EulerSensor::EulerSensor(const string& name, DataSource* datasrc) : Sensor(name, datasrc)
	{

	}

	EulerSensor::~EulerSensor()
	{

	}

	void EulerSensor::init()
	{
		m_data = m_Euler;
	}

	void EulerSensor::update()
	{
		m_dataSrc->getEuler(name(), m_Euler);
	}



	QuaternionSensor::QuaternionSensor(const string& name, DataSource* datasrc) : Sensor(name, datasrc)
	{

	}

	QuaternionSensor::~QuaternionSensor()
	{

	}

	void QuaternionSensor::init()
	{
		m_data = m_quaternion;
	}

	void QuaternionSensor::update()
	{
		m_dataSrc->getQuartenion(name(), m_quaternion);
	}
}