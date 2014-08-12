#pragma once
#include "ControlInterface.h"
#include "RateLimiter.h"
#include "XboxJoystick.h"

class CJoystickControl : public ControlInterface
{
public:
	CJoystickControl();
	~CJoystickControl();

	virtual void update();
	virtual void getCommand(short& linear, short& angular);


private:
	XboxJoystick m_xboxJoyStick;
	bool m_bJoystickFlag;
	RateLimiter m_rateLimiter;

};

