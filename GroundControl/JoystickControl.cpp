#include "stdafx.h"
#include "JoystickControl.h"



CJoystickControl::CJoystickControl() : m_rateLimiter(50, Eigen::Vector2f(0, 0))
{
	m_xboxJoyStick.setMax(1000);
}


CJoystickControl::~CJoystickControl()
{
}

void CJoystickControl::update()
{
	
}

void CJoystickControl::getCommand(short& linear, short& angular)
{

	short x = 0, y = 0;
	if (m_xboxJoyStick.update())
	{
		if (m_xboxJoyStick.getButtonValue(X_BUTTON))
		{
			m_xboxJoyStick.getStickValue(LEFT_STICK, x, y);
		}
	}
		
	Eigen::Vector2f val = m_rateLimiter(Eigen::Vector2f(x, y));

	linear = 0.5*val[1];
	angular = -val[0];
	
}