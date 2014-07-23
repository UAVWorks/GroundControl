#include "stdafx.h"
#include "IMUSensorManager.h"


namespace GroundControl
{

	IMUSensorManager::IMUSensorManager()
	{
	}


	IMUSensorManager::~IMUSensorManager()
	{
		for (list<IMUSensorInfo>::iterator iter = m_IMUSensors.begin(); iter != m_IMUSensors.end(); ++iter)
		{
			delete iter->imu;
		}
		
	}



	bool IMUSensorManager::init()
	{
		return true;
	}

	void IMUSensorManager::quit()
	{
	}

	bool IMUSensorManager::update()
	{
		for (list<IMUSensorInfo>::iterator iter = m_IMUSensors.begin(); iter != m_IMUSensors.end(); ++iter)
		{
			IMUSensor *imu = iter->imu;
			imu->update();
		}

		return true;
	}

	bool IMUSensorManager::getPos(const string& ID, float* pos) const
	{
		return true;
	}

	bool IMUSensorManager::getRot(const string& ID, float* rot) const
	{
		return true;
	}

	bool IMUSensorManager::getEuler(const string& ID, float* Euler) const
	{
		return true;
	}

	bool IMUSensorManager::getQuartenion(const string& ID, float* quaternion) const
	{
		for (list<IMUSensorInfo>::const_iterator iter = m_IMUSensors.begin(); iter != m_IMUSensors.end(); ++iter)
		{
			if (iter->name == ID)
			{
				quaternion[0] = iter->imu->read(quaternion);
				break;
			}
		}
				
		return true;
	}

	void IMUSensorManager::add(IMUSensor* sensor, const string& name)
	{
		IMUSensorInfo info;
		info.name = name;
		info.imu = sensor;

		m_IMUSensors.push_back(info);
	}

}