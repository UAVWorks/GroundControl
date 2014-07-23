#include "stdafx.h"
#include "ViconMotionCapture.h"


namespace GroundControl
{
	ViconMotionCapture::ViconMotionCapture()
	{
	}


	ViconMotionCapture::~ViconMotionCapture()
	{
	}


	bool ViconMotionCapture::init()
	{
		string hostName = "10.42.0.4:801";

		Output_Connect rConnect = m_viconClient.Connect(hostName);
		if (rConnect.Result != Result::Success)
		{
			return false;
		}

		m_viconClient.EnableSegmentData();
		m_viconClient.SetStreamMode(StreamMode::ClientPullPreFetch);

		return true;
	}

	void ViconMotionCapture::quit()
	{
		m_viconClient.DisableSegmentData();
		m_viconClient.Disconnect();
	}

	bool ViconMotionCapture::update()
	{
		bool bRes = true;

		Output_GetFrame rGetFrame = m_viconClient.GetFrame();
		if (rGetFrame.Result != Result::Success)
		{
			bRes = false;
		}

		return bRes;
	}

	bool ViconMotionCapture::getPos(const string& ID, float* pos) const
	{
		bool bOk = false;
		Eigen::Vector3f lPos = getPos(ID, &bOk);
		if (!bOk)
			return false;
		
		pos[0] = lPos[0];
		pos[1] = lPos[1];
		pos[2] = lPos[2];

		return true;
	}

	bool ViconMotionCapture::getRot(const string& ID, float* rot) const
	{
		bool bOk = false;
		Eigen::Matrix3f lRot = getRot(ID, &bOk);
		if (!bOk)
			return false;

		for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rot[3 * i + j] = lRot(i, j);

		return true;

	}

	bool ViconMotionCapture::getEuler(const string& ID, float* Euler) const
	{
		bool bOk = false;
		Eigen::Vector3f lEuler = getEuler(ID, &bOk);
		if (!bOk)
			return false;

		Euler[0] = lEuler[0];
		Euler[1] = lEuler[1];
		Euler[2] = lEuler[2];

		return true;

	}

	bool ViconMotionCapture::getQuartenion(const string& ID, float* quaternion) const
	{
		bool bOk = false;
		Eigen::Vector4f lQ = getQuartenion(ID, &bOk);
		if (!bOk)
			return false;

		quaternion[0] = lQ[0];
		quaternion[1] = lQ[1];
		quaternion[2] = lQ[2];
		quaternion[3] = lQ[3];

		return true;

	}


	Eigen::Vector3f ViconMotionCapture::getPos(const string& ID, bool* bOk) const
	{
		Output_GetSegmentGlobalTranslation rPos = m_viconClient.GetSegmentGlobalTranslation(ID, ID);
		if (rPos.Result != Result::Success)
		{
			if (bOk != NULL)
				*bOk = false;

			return Eigen::Vector3f(0, 0, 0);
		}

		if (bOk != NULL)
			*bOk = true;

		return Eigen::Vector3f(rPos.Translation[0] / 1000, rPos.Translation[1] / 1000, rPos.Translation[2] / 1000);
		
	}

	Eigen::Vector3f ViconMotionCapture::getEuler(const string& ID, bool* bOk) const
	{
		Output_GetSegmentGlobalRotationEulerXYZ r = m_viconClient.GetSegmentGlobalRotationEulerXYZ(ID, ID);
	
		if (r.Result != Result::Success)
		{
			if (bOk != NULL)
				*bOk = false;

			return Eigen::Vector3f(0, 0, 0);
		}

		if (bOk != NULL)
			*bOk = true;

		return Eigen::Vector3f(r.Rotation[0], r.Rotation[1], r.Rotation[1]);

	}
		
	Eigen::Vector4f ViconMotionCapture::getQuartenion(const string& ID, bool* bOk) const
	{

		Output_GetSegmentGlobalRotationQuaternion q = m_viconClient.GetSegmentGlobalRotationQuaternion(ID, ID);
		if (q.Result != Result::Success) {
			if (bOk != NULL) *bOk = false;
			return Eigen::Vector4f(0,0,0,0) ;
		}


		if (bOk != NULL)
			*bOk = true;

		return Eigen::Vector4f(q.Rotation[0], q.Rotation[1], q.Rotation[1], q.Rotation[3]);
	}


	Eigen::Matrix3f ViconMotionCapture::getRot(const string& ID, bool* bOk) const
	{

		Eigen::Matrix3f pose;


		Output_GetSegmentGlobalRotationMatrix r = m_viconClient.GetSegmentGlobalRotationMatrix(ID, ID);
		if (r.Result != Result::Success) {
			if (bOk != NULL) *bOk = false;
			return Eigen::Matrix3f::Identity();
		}
		//    qDebug("%f %f %f %f %f %f %f %f %f",
		//           r.Rotation[0], r.Rotation[1], r.Rotation[2],
		//           r.Rotation[3], r.Rotation[4], r.Rotation[5],
		//           r.Rotation[6], r.Rotation[7], r.Rotation[8]);



		pose << r.Rotation[0], r.Rotation[1], r.Rotation[2],
			r.Rotation[3], r.Rotation[4], r.Rotation[5],
			r.Rotation[6], r.Rotation[7], r.Rotation[8];

		if (bOk != NULL) *bOk = true;
		return pose;

	}


}

