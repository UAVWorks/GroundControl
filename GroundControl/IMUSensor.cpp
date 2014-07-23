#include "stdafx.h"
#include "IMUSensor.h"


namespace GroundControl
{


	IMUSensor::IMUSensor() 
	{
		m_data[0] = m_data[1] = m_data[2] = m_data[3] = 0;
	}


	IMUSensor::~IMUSensor()
	{
	}

	void IMUSensor::update()
	{
		int len = 0;
		bool res = false;
		char buffer[100];

		BYTE c = 0;

		while (c != '#')
		{
			c = m_com.getChar();
			if (c == -1)
				return ;
		}

		int idx = 0;
		while (c != '\r')
		{
			c = m_com.getChar();
			if (c == -1)
				return ;
			buffer[idx++] = c;
		}

		int num = idx;
		int dataIdx = 0;
		int startIdx = 0;
		idx = 0;


		while (idx < num)
		{
			if (buffer[idx] == ',' || buffer[idx] == '\r')
			{
				buffer[idx] = '\0';
				m_data[dataIdx] = atof(buffer + startIdx);
				dataIdx++;
				startIdx = idx + 1;
			}

			idx++;
		}
		/*if (m_callback)
		{
			m_callback(data, m_callbackParam);
		}*/


		return;

	}


	bool IMUSensor::read(float* data)
	{
	
		memcpy(data, m_data, 4 * sizeof(float));
	
		return true;

	}

}