#include "stdafx.h"
#include "Message.h"


namespace GroundControl
{
	Message::Message() : m_payload(NULL)
	{

	}

	Message::~Message(){}

	GeneralMessage::GeneralMessage(u_short id, byte* data, u_short datasize)
	{
		if (datasize != 0)
		{
			m_payload = new byte[datasize];
			memcpy(m_payload, data, datasize);
		}
	
		m_id = id;
		m_size = datasize;
	}

	GeneralMessage::~GeneralMessage()
	{
		if (m_payload)
			delete m_payload;
	}

}





