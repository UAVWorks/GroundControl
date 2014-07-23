#pragma once
#include <string>
using namespace std;

namespace GroundControl
{
	enum eDataSrcType
	{
		eDataSrcType_Vicon,
		eDataSrcType_IMU, 
	};
	
	class DataSource
	{
	public:
		DataSource();
		virtual ~DataSource();

		virtual bool init() = 0;
		virtual void quit() = 0;
		virtual bool update() = 0;

		virtual bool getPos(const string& ID, float* pos) const = 0;
		virtual bool getRot(const string& ID, float* rot) const = 0;
		virtual bool getEuler(const string& ID, float* Euler) const = 0;
		virtual bool getQuartenion(const string& ID, float* quaternion) const = 0;
	};

}