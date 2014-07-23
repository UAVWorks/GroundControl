#pragma once
#include "Element.h"
#include "DataSource.h"


namespace GroundControl
{

	enum eSensorType
	{
		eSensorType_Pos,
		eSensorType_Rotation,
		eSensorType_Euler,
		eSensorType_Quaternion, 
	};


	class Sensor : public Element
	{
	public:
		Sensor(const string& name, DataSource* datasrc);
		virtual ~Sensor();

		const float* read() const;
		virtual void update() = 0;
		virtual void init() = 0;
		virtual eSensorType getType() const = 0;

	protected:
		float* m_data;
		DataSource* m_dataSrc;

	};

	class PosSensor : public Sensor
	{
	public:
		PosSensor(const string& name, DataSource* dataSrc);
		virtual ~PosSensor();

		virtual void init();
		virtual void update();
		virtual eSensorType getType() const { return eSensorType_Pos;  }

	protected:
		float m_posData[3];
		
	};

	class RotationSensor : public Sensor
	{
	public:
		RotationSensor(const string& name, DataSource* datasrc);
		virtual ~RotationSensor();

		virtual void init();
		virtual void update();
		virtual eSensorType getType() const { return eSensorType_Rotation; }

	protected:
		float m_rotation[9];
	};

	class EulerSensor : public Sensor
	{
	public:
		EulerSensor(const string& name, DataSource* datasrc);
		virtual ~EulerSensor();

		virtual void init();
		virtual void update();
		virtual eSensorType getType() const { return eSensorType_Euler; }

	protected:
		float m_Euler[3];

	};

	class QuaternionSensor : public Sensor
	{
	public:
		QuaternionSensor(const string& name, DataSource* datasrc);
		virtual ~QuaternionSensor();

		virtual void init();
		virtual void update();
		virtual eSensorType getType() const { return eSensorType_Quaternion; }

	protected:
		float m_quaternion[4];

	};


}