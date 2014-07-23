#pragma once
#include "Node.h"
#include "ViconMotionCapture.h"
#include <list>
#include <vector>
#include "IMUSensorManager.h"

using namespace std;

namespace GroundControl
{
	class SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		void init();
		void update();
		void quit();

		const list<Node*>& getNodeList() const;
		list<Node*>& getNodeList();
		const Node* findNode(const string& name) const;

		friend void ThreadFunc(void* param);

	private:
		Sensor* createSensor(const string& name, int type, int datasrc);

	private:
		list<Node*> m_nodelist;
		ViconMotionCapture m_vicon;
		IMUSensorManager m_IMUSensors;
		

		HANDLE m_hThread;
		bool m_bQuit;

	};

}

