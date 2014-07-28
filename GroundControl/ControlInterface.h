#pragma once
class ControlInterface
{
public:
	ControlInterface();
	virtual ~ControlInterface();

	virtual void update() = 0;
	virtual void getCommand(short& linear, short& angular) = 0;
};

