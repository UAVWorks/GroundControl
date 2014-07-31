#pragma once
#include "XInput.h"

#define LEFT_STICK 0
#define RIGHT_STICK 1

#define A_BUTTON 0x1000
#define B_BUTTON 0x2000
#define X_BUTTON 0x4000
#define Y_BUTTON 0x8000 

class XboxJoystick 
{
public:
	XboxJoystick();
	XboxJoystick(DWORD id);
	~XboxJoystick();


	void getStickValue(int which, short& xVal, short& yVal);
	bool getButtonValue(int which);
	void setMax(u_short maxVal);
	bool update();
	

private:
	u_short m_maxVal;
	XINPUT_STATE m_state;
	DWORD m_id;
};

