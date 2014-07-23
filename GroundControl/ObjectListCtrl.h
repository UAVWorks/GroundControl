#pragma once

#include <Eigen/Core>


class CObjectListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CObjectListCtrl)

public:
	CObjectListCtrl();
	virtual ~CObjectListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int InsertObjectData(const CString& name, const float* pos, const float* rot, const float* euler, const float* q);
	void UpdateObjectData(const CString& name, const float* pos, const float* rot, const float* euler, const float* q);

private:
	int m_dataID;

	void SetObjectData(int idx, const float* pos, const float* rot, const float* euler, const float* q);


public:
	void update();

};


