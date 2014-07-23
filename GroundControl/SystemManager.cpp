#include "stdafx.h"
#include "SystemManager.h"
#include "MsgComm.h"
#include "Tranceiver.h"
#include "Config.h"


#define _TEST 0



namespace GroundControl
{


#ifdef _TEST
	UDPTranceiver sock(7150, "127.0.0.1", 7250);
	float posdata[6];
#endif



	void ThreadFunc(void* param)
	{	
		SystemManager* system = (SystemManager*)param;

		while (!system->m_bQuit)
		{
			system->update();
			Sleep(30);
		}
	}

	SystemManager::SystemManager() : m_bQuit(false)
	{
	}


	SystemManager::~SystemManager()
	{
	}


	void SystemManager::init()
	{
		
	//	m_vicon.init();
	//	m_IMUSensors.init();

		ConfigLoader loader;
		loader.LoadConfig("config.xml", NULL);
			
		int IMUNum = loader.m_data.IMUs.size();
		for (int i = 0; i < IMUNum; i++)
		{
			IMUSensor* imu = new IMUSensor();
			imu->init(loader.m_data.IMUs[i].port, loader.m_data.IMUs[i].buadrate);
			m_IMUSensors.add(imu, loader.m_data.IMUs[i].name);
		}
		
		int nodeNum = loader.m_data.nodeinfoes.size();
		for (int i = 0; i < nodeNum; i++)
		{
			ConfigData::NodeInfo& nodeinfo = loader.m_data.nodeinfoes[i];
			Node* node = new Node(nodeinfo.name);

			int sensorsize = nodeinfo.sensors.size();
			for (int j = 0; j < sensorsize; j++)
			{
				node->addSensor(createSensor(nodeinfo.name, nodeinfo.sensors[j].type, nodeinfo.sensors[j].srctype));
			}

			if (nodeinfo.comm.type == eTranceiverType_UDP)
			{
				Tranceiver* t = new UDPTranceiver(nodeinfo.comm.port, nodeinfo.comm.remote_addr, nodeinfo.comm.remote_port);
				MsgComm* comm = new MsgComm(t);
				node->registerMsgComm(comm);
			}

			m_nodelist.push_back(node);
		}

		m_hThread =(HANDLE) _beginthread(ThreadFunc, 0, this);

	}
	void SystemManager::update()
	{
		//m_vicon.update();
		//m_IMUSensor.update();
		for (list<Node*>::iterator iter = m_nodelist.begin(); iter != m_nodelist.end(); ++iter)
		{
			Node* node = *iter;
			node->update();
		}

#ifdef _TEST

		int idx = 0;

		for (list<Node*>::iterator iter = m_nodelist.begin(); iter != m_nodelist.end(); ++iter)
		{
			Node* node = *iter;
			const float* pos = node->pos();
			memcpy(posdata + 3 * idx, pos, 3 * sizeof(float));
			idx++;
		}

		sock.send(100, (const byte*)posdata, 6 * sizeof(float));

#endif

	}

	void SystemManager::quit()
	{
	//	m_vicon.quit();
	//	m_IMUSensor.quit();
		m_bQuit = true;

		WaitForSingleObject(m_hThread, INFINITE);
	}

	const list<Node*>& SystemManager::getNodeList() const
	{
		return m_nodelist;
	}

	list<Node*>& SystemManager::getNodeList() 
	{
		return m_nodelist;
	}

	const Node* SystemManager::findNode(const string& name) const
	{
		for (list<Node*>::const_iterator iter = m_nodelist.begin(); iter != m_nodelist.end(); ++iter)
		{
			if ((*iter)->name() == name)
				return *iter;
		}
		return NULL;
	}

	Sensor* SystemManager::createSensor(const string& name, int type, int dataSrcType)
	{
		Sensor* sensor = NULL;
		DataSource* datasrc = NULL;

		switch(dataSrcType)
		{
			case eDataSrcType_Vicon:
				datasrc = &m_vicon;
				break;

			case eDataSrcType_IMU:
				datasrc = &m_IMUSensors;
				break;
		}


		switch (type)
		{
		case eSensorType_Pos:
			sensor = new PosSensor(name, datasrc);
			break;

		case eSensorType_Euler:
			sensor = new EulerSensor(name, datasrc);
			break;

		case eSensorType_Rotation:
			sensor = new RotationSensor(name, datasrc);
			break;

		case eSensorType_Quaternion:
			sensor = new QuaternionSensor(name, datasrc);
			break;
		}

		return sensor;
	}



}


