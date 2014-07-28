#include "stdafx.h"
#include "XboxJoystick.h"


XboxJoystick::XboxJoystick() : m_id(0), m_maxVal(0)
{
	memset(&m_state, 0, sizeof(XINPUT_STATE));
}

XboxJoystick::XboxJoystick(DWORD id) : m_id(id), m_maxVal(0)
{
	memset(&m_state, 0, sizeof(XINPUT_STATE));
}


XboxJoystick::~XboxJoystick()
{
}


void XboxJoystick::getStickValue(int which, short& xVal, short& yVal)
{
	int x = 0, y = 0;

	if (which == LEFT_STICK)
	{
		x = m_state.Gamepad.sThumbLX;
		y = m_state.Gamepad.sThumbLY;
	}
	else if (which == RIGHT_STICK)
	{
		x = m_state.Gamepad.sThumbRX;
		y = m_state.Gamepad.sThumbRY;
	}

	if (m_maxVal != 0)
	{
		x = (x*m_maxVal) >> 15;
		y = (y*m_maxVal) >> 15;
	}
		
	xVal = x;
	yVal = y;

}

bool XboxJoystick::getButtonValue(int which)
{
	return ((m_state.Gamepad.wButtons & which) != 0);
}

void XboxJoystick::setMax(u_short maxVal)
{
	m_maxVal = maxVal;
}

bool XboxJoystick::update()
{
	bool bRes = true;
	DWORD dwResult = XInputGetState(m_id, &m_state);
	if (dwResult != 0)
		bRes = false;

	return bRes;
		
}
