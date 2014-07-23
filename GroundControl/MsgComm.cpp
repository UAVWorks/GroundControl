#include "stdafx.h"
#include "Message.h"
#include "Tranceiver.h"
#include "MsgComm.h"


namespace GroundControl
{

	MsgComm::MsgComm(Tranceiver* tranceiver) : m_tranceiver(tranceiver)
	{
		m_tranceiver->registerMsgComm(this);
	}


	MsgComm::~MsgComm()
	{
		delete m_tranceiver;
	}


	void MsgComm::sendMsg(const Message* msg)
	{
		m_tranceiver->send(msg->id(), msg->payload(), msg->size());
	}

	void MsgComm::onMessage(u_short id, const byte* payload, u_short size)
	{
		switch (id)
		{
		default:

			break;
		}
	}

	void MsgComm::receive()
	{
		m_tranceiver->receive();
	}

}
