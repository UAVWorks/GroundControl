#include "stdafx.h"
#include "MsgComm.h"
#include "Tranceiver.h"


namespace GroundControl
{


	Tranceiver::Tranceiver()
	{
	}


	Tranceiver::~Tranceiver()
	{
	}


	void Tranceiver::registerMsgComm(MsgComm* comm)
	{
		m_msgComm = comm;
	}


	UDPTranceiver::UDPTranceiver(u_short port, const string& remoteAddr, u_short remotePort)
	{
		create(false, kaiON_MESSAGE);
		bind(port);
		connect(remoteAddr.c_str(), remotePort);
		setBlockingMode(kaiNON_BLOCKING_SOCKET);
	}

	UDPTranceiver::~UDPTranceiver()
	{
		close();
	}

	void UDPTranceiver::send(u_short id, const byte* data, u_short size)
	{
		kaiMsg msg;
		msg.allocateMemory(size + 8);
		msg.begin();
		msg.id(id);
		msg.writeData(data, size);
		msg.end();

		kaiSocket::send(msg);

	}

	void UDPTranceiver::receive()
	{
		recv();
	}

	void UDPTranceiver::onMessage(kaiMsg& msg)
	{
		if (m_msgComm)
		{
			m_msgComm->onMessage(msg.id(), (const byte*)(msg.buffer() + Size_kaiHEADER), msg.dataSize());
		}
	}


}