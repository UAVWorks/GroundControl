#pragma once
#include <string>

using namespace std;


namespace GroundControl
{

	class Element
	{
	public:
		Element();
		Element(const string& name);
		virtual ~Element();


		bool operator== (const Element& info) const
		{
			return (name() == info.name());
		}

		bool operator== (const string& _name) const
		{
			return (name() == _name);
		}

		bool operator != (const Element& info) const
		{
			return (name() != info.name());
		}

		bool operator!= (const string& _name) const
		{
			return (name() != _name);
		}


		const string& name() const { return m_name; }

	private:
		string m_name;

	};

}