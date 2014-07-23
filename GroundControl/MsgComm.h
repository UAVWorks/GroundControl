#pragma once



namespace GroundControl
{
	class Message;
	class Tranceiver;

	class MsgComm
	{
	public:
		MsgComm(Tranceiver* tranceiver);
		~MsgComm();

		void sendMsg(const Message* msg);
		void onMessage(u_short id, const byte* payload, u_short size);
		void receive();

	private:
		Tranceiver* m_tranceiver;
		
	};

}


