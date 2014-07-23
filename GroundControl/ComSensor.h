#pragma once

#include "ComDriver.h"
#include <string>


using namespace std;


namespace GroundControl
{

typedef void(*ComSensorCallback) (const float*, void*);


class ComSensor 
{
public:
	ComSensor();
	virtual ~ComSensor();
	
	virtual bool read(float* data) = 0;
	virtual void update() = 0;

	int init(int port, int baudrate);
	void quit();
	void registerSensorCallback(ComSensorCallback callback, void* param);

protected:

	ComDriver m_com;
	ComSensorCallback m_callback;
	void* m_callbackParam;


};

}
