#pragma once
#include "IMUSensor.h"
#include "DataSource.h"
#include <list>
#include <string>

using namespace std;

namespace GroundControl
{


	class IMUSensorManager :
		public DataSource
	{
	public:
		IMUSensorManager();
		~IMUSensorManager();

		struct IMUSensorInfo
		{
			IMUSensor* imu;
			string name;

		};

		virtual bool init();
		virtual void quit();
		virtual bool update();

		virtual bool getPos(const string& ID, float* pos) const;
		virtual bool getRot(const string& ID, float* rot) const;
		virtual bool getEuler(const string& ID, float* Euler) const;
		virtual bool getQuartenion(const string& ID, float* quaternion) const;

		void add(IMUSensor* sensor, const string& name);


	private:
		list<IMUSensorInfo> m_IMUSensors;

	};

}