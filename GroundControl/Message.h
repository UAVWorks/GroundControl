#pragma once

namespace GroundControl
{
	class Message
	{
	public:
		Message();
		virtual ~Message() = 0;


		const byte* payload() const { return m_payload; }
		u_short size() const { return m_size; }
		u_short id() const { return m_id; }

	protected:
		byte* m_payload;
		u_short m_size;
		u_short m_id;

	};



	class GeneralMessage : public Message
	{
	public:
		class GeneralMessage(u_short id, byte* data, u_short datasize);
		~GeneralMessage();

	};
	

}