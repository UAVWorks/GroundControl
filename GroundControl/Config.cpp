#include "StdAfx.h"
#include "stdio.h"
#include "config.h"

#ifdef _WIN32
#   define INLINE __inline
#elif defined(__GNUC__)
#   define INLINE inline
#else
#   define INLINE
#endif

#ifndef CHI_TRACK_MEM
#ifdef _DEBUG
#define new DEBUG_NEW
#endif // _DEBUG
#endif // CHI_TRACK_MEM




void xml_start_handler(void* userData, const XML_Char* name, const XML_Char** attr);
void xml_end_handler(void* userData, const XML_Char* name);

bool parse_bool(const char*& s);
float parse_atof(const char*& s);
float parse_atofx(const char*& s, int& len);
int parse_atoi(const char*& s);
int parse_atoix(const char*& s, int& len);

ConfigLoader::ConfigLoader()
:
m_parser(NULL),
m_userData(NULL)
{
	memset(&m_data, 0, sizeof(m_data));
}


bool ConfigLoader::LoadConfig(char* config, void* userData)
{
	char buffer[1024];
	FILE* file = NULL;
	fopen_s(&file, config, "rb");
	if (!file)
		return false;

	m_userData = userData;
	m_parser = XML_ParserCreate(NULL);
	XML_SetUserData(m_parser, this);
	XML_SetElementHandler(m_parser, xml_start_handler, xml_end_handler);
	XML_SetCharacterDataHandler(m_parser, NULL);

	bool error = false;
	int total = 0;
	int read = 0;
	while (!feof(file))
	{
		read = fread(buffer, sizeof(char), 1024, file);
		if (ferror(file))
		{
			error = true;
			break;
		}
		total += read;

		if (XML_Parse(m_parser, buffer, read, feof(file)) == XML_STATUS_ERROR)
		{
			printf("Parse error at line %d:\n%s\n",
				XML_GetCurrentLineNumber(m_parser), 
				XML_ErrorString(XML_GetErrorCode(m_parser)));
			error = true;
			break;
		}
	}

	XML_ParserFree(m_parser);
	fclose(file);

	return !error;
}

// 항목을 추가되면 이곳에 코드를 추가합니다. 
void xml_start_handler(void* userData, const XML_Char* name, const XML_Char** attr)
{
	static int IMU_idx = 0;
	static int node_idx = 0;
	static int sensor_idx = 0;

	if (strcmp(name, "IMU") == 0)
	{
		ConfigData::IMUInfo imu;
		while (attr[0])
		{
			if (strcmp(attr[0], "port") == 0)
			{
				imu.port = atoi(attr[1]);
			}
			else if (strcmp(attr[0], "node") == 0)
			{
				imu.name = attr[1];
			}
			else if (strcmp(attr[0], "baudrate") == 0)
			{
				imu.buadrate = atoi(attr[1]);
			}
			attr += 2;
		}

		((ConfigLoader*)userData)->m_data.IMUs.push_back(imu);
	}
	else if (strcmp(name, "node") == 0)
	{
		ConfigData::NodeInfo node;
		while (attr[0])
		{
			if (strcmp(attr[0], "name") == 0)
			{
				node.name = attr[1];
			}
			attr += 2;
		}

		((ConfigLoader*)userData)->m_data.nodeinfoes.push_back(node);
		
	}
	else if (strcmp(name, "sensor") == 0)
	{
		ConfigData::SensorInfo sensor;
		while (attr[0])
		{
			if (strcmp(attr[0], "source") == 0)
			{
				sensor.srctype = atoi(attr[1]);
			}
			else if (strcmp(attr[0], "type") == 0)
			{
				sensor.type = atoi(attr[1]);
			}
			attr += 2;
		}
		int idx = ((ConfigLoader*)userData)->m_data.nodeinfoes.size() - 1;
		((ConfigLoader*)userData)->m_data.nodeinfoes[idx].sensors.push_back(sensor);
	}
	else if (strcmp(name, "UDP") == 0)
	{
		int idx = ((ConfigLoader*)userData)->m_data.nodeinfoes.size() - 1;
		ConfigData::CommInfo& info = ((ConfigLoader*)userData)->m_data.nodeinfoes[idx].comm;
		info.type = 1;

		while (attr[0])
		{
			if (strcmp(attr[0], "port") == 0)
			{
				info.port = atoi(attr[1]);
			}
			else if (strcmp(attr[0], "remote_addr") == 0)
			{
				info.remote_addr = attr[1];
			}
			else if (strcmp(attr[0], "remote_port") == 0)
			{
				info.remote_port = atoi(attr[1]);
			}
			attr += 2;

		}
	}
	
}

void xml_end_handler(void* userData, const XML_Char* name)
{
}



// helpers

INLINE bool parse_bool(const char*& s)
{
	if (strcmp(s, "true") == 0)
		return true;
	else 
		return false;
}

INLINE float parse_atof(const char*& s)
{
	int numeric = 0;
	float decimal = 0.0f;
	float x = 0.1f;
	float neg = 1.0f;

	if (*s == '-')
	{
		neg = -1.0f;
		s++;
	}

	while ((*s >= '0') && (*s <= '9'))
	{
		numeric = (numeric * 10) + (*s - '0');
		s++;
	}

	if (*s == '.')
	{
		s++;

		while ((*s >= '0') && (*s <= '9'))
		{
			decimal += (*s - '0') * x;
			x *= 0.1f;
			s++;
		}
	}

	return neg * (numeric + decimal);
}

INLINE float parse_atofx(const char*& s, int& len)
{
	int numeric = 0;
	float decimal = 0.0f;
	float x = 0.1f;
	float neg = 1.0f;

	if (len && (*s == '-'))
	{
		neg = -1.0f;
		s++; len--;
	}

	while (len && (*s >= '0') && (*s <= '9'))
	{
		numeric = (numeric * 10) + (*s - '0');
		s++; len--;
	}

	if (len && (*s == '.'))
	{
		s++; len--;

		while (len && (*s >= '0') && (*s <= '9'))
		{
			decimal += (*s - '0') * x;
			x *= 0.1f;
			s++; len--;
		}
	}

	return neg * (numeric + decimal);
}

INLINE int parse_atoi(const char*& s)
{
	int numeric = 0, neg = 1;

	if (*s == '-')
	{
		neg = -1;
		s++;
	}

	while ((*s >= '0') && (*s <= '9'))
	{
		numeric = (numeric * 10) + (*s - '0');
		s++;
	}

	return neg * numeric;
}

INLINE int parse_atoix(const char*& s, int& len)
{
	int numeric = 0, neg = 1;

	if (len && (*s == '-'))
	{
		neg = -1;
		s++; len--;
	}

	while (len && (*s >= '0') && (*s <= '9'))
	{
		numeric = (numeric * 10) + (*s - '0');
		s++; len--;
	}

	return neg * numeric;
}