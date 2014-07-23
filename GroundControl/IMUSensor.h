#pragma once

#include "ComDriver.h"
#include "ComSensor.h"

#include <vector>


namespace GroundControl
{

	class IMUSensor : public ComSensor
	{
	public:

	

		IMUSensor();
		~IMUSensor();

	
		virtual bool read(float* data);
		virtual void update();


	private:
		float m_data[4];

	};

}

