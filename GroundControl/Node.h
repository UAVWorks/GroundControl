#pragma once
#include <string>
#include <list>
#include "Sensor.h"
#include <Eigen/Core>
#include "Element.h"

using namespace std;

namespace GroundControl
{
		
	class MsgComm;
	class Message;

	class Node : public Element
	{
	public:
		Node();
		Node(const string& name);
		virtual ~Node();


	
		const float* pos() const { return m_pos; }
		const float* rot() const { return m_rot; }
		const float* Euler() const { return m_Euler; }
		const float* Quarternion() const { return m_quaternion; }

		void addSensor(Sensor* sensor);
		void registerMsgComm(MsgComm* comm) { m_comm = comm; }
				
		void sendCommand(const Message* msg);

		void update();
		void init();

	private:		
		float m_pos[3];
		float m_rot[3];
		float m_Euler[3];
		float m_quaternion[4];

		list<Sensor*> m_sensors;
		MsgComm* m_comm;

	};

}

