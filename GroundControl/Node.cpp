#include "stdafx.h"
#include "Message.h"
#include "MsgComm.h"
#include "Node.h"


namespace GroundControl
{
	Node::Node()
	{
	}

	Node::Node(const string& name) : Element(name), m_comm(NULL)
	{
	}


	Node::~Node()
	{
		for (list<Sensor*>::iterator iter = m_sensors.begin(); iter != m_sensors.end(); ++iter)
		{
			delete (*iter);
		}

		if (m_comm)
			delete m_comm;
	}


	void Node::init()
	{
		memset(m_pos, 0, 3 * sizeof(float));
		memset(m_Euler, 0, 3 * sizeof(float));
		memset(m_rot, 0, 9 * sizeof(float));
		memset(m_quaternion, 0, 4 * sizeof(float));
	}

	void Node::addSensor(Sensor* sensor)
	{
		sensor->init();
		m_sensors.push_back(sensor);
	}

	void Node::sendCommand(const Message* msg)
	{
		if (m_comm)
			m_comm->sendMsg(msg);
	}

	void Node::update()
	{
		if (m_comm)
			m_comm->receive();

		for (list<Sensor*>::iterator iter = m_sensors.begin(); iter != m_sensors.end(); ++iter)
		{
			Sensor* sensor = *iter;
			sensor->update();
			const float* data = sensor->read();
		
			if (data == NULL)
				continue; 

			switch (sensor->getType())
			{
			case eSensorType_Pos:
				memcpy(m_pos, data, 3*sizeof(float));
				break;

			case eSensorType_Euler:
			{
				memcpy(m_Euler, data, 3*sizeof(float));

			}
				break;
			case eSensorType_Rotation:
			{
				memcpy(m_rot, data, 9*sizeof(float));
			}
				break;

			case eSensorType_Quaternion:
			{
				memcpy(m_quaternion, data, 4*sizeof(float));
			}
				break;
			}

		}
	}

}
