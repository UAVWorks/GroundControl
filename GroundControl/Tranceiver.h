#pragma once
#include "kai/kai.h"

namespace GroundControl
{
	class MsgComm;

	enum eTranceiverType
	{
		eTranceiverType_UDP = 1, 

	};

	class Tranceiver
	{
	public:
		Tranceiver();
		virtual ~Tranceiver();

		void registerMsgComm(MsgComm* comm);


		virtual void send(u_short id, const byte* data, u_short size) = 0;
		virtual void receive() = 0;

	protected:
		MsgComm* m_msgComm;
		

	};

	class UDPTranceiver : public Tranceiver, kaiSocket
	{
	public:
		UDPTranceiver(u_short port, const string& remoteAddr, u_short remotePort);
		~UDPTranceiver();

		virtual void send(u_short id, const byte* data, u_short size);
		virtual void receive();
		virtual void onMessage(kaiMsg& msg); 

	private:
	
	};

	
}