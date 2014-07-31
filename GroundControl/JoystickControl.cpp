#include "stdafx.h"
#include "JoystickControl.h"



CJoystickControl::CJoystickControl()
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
	linear = 0;
	angular = 0;

	if (m_xboxJoyStick.update())
	{
		if (m_xboxJoyStick.getButtonValue(X_BUTTON))
		{
			m_xboxJoyStick.getStickValue(LEFT_STICK, linear, angular);
		}
	}
	
}