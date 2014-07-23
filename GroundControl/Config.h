#pragma once
#include "expat.h"
#include <string>
#include <vector>

using namespace std;


// config.xml���Ϸκ��� ���� ������ �о���Դϴ�.

struct ConfigData // ���� �׸��� ���� �̰��� �׸��� �߰��մϴ�. 
{
	struct IMUInfo
	{
		string name;
		int port;
		int buadrate;
	};

	struct CommInfo
	{
		CommInfo()
		{
			type = 0;
		}

		int type;
		u_short port;
		string remote_addr;
		u_short remote_port;
	};

	struct SensorInfo
	{
		int type;
		int srctype;
	};

	struct NodeInfo
	{
		string name;
		vector<SensorInfo> sensors;
		CommInfo comm;
	};

	vector<NodeInfo> nodeinfoes;
	vector<IMUInfo> IMUs;
};

class ConfigLoader 
{
public:
	ConfigLoader();

public:
	bool LoadConfig(char* config, void* userData);
	void* GetUserData() { return m_userData; }
	XML_Parser GetParser() { return m_parser; }

private:
	XML_Parser m_parser;
	void* m_userData;

public:
	ConfigData m_data;
};

