#pragma once

#include <Eigen/Core>
#include "Client.h"
#include "DataSource.h"
#include <string>



using namespace ViconDataStreamSDK::CPP;
using namespace std;

namespace GroundControl 
{

	class ViconMotionCapture : public DataSource
	{
	public:
		ViconMotionCapture();
		~ViconMotionCapture();


		virtual bool init();
		virtual void quit();
		virtual bool update();


		virtual bool getPos(const string& ID, float* pos) const;
		virtual bool getRot(const string& ID, float* rot) const;
		virtual bool getEuler(const string& ID, float* Euler) const;
		virtual bool getQuartenion(const string& ID, float* quaternion) const;

	private:

		Eigen::Vector3f getPos(const string& ID, bool* bOk = NULL) const;
		Eigen::Matrix3f getRot(const string& ID, bool* bOk = NULL) const;
		Eigen::Vector3f getEuler(const string& ID, bool* bOk = NULL) const;
		Eigen::Vector4f getQuartenion(const string& ID, bool* bOk = NULL) const;


	private:
		Client m_viconClient;
		float m_q[4];
	};

}

