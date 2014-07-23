#pragma once

#include "MyXYGraph.h"
#include "ControlPanel.h"
#include "ObjectListCtrl.h"

class MyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(MyTabCtrl)

public:
	MyTabCtrl();
	virtual ~MyTabCtrl();

protected:

	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);


private:
	CObjectListCtrl m_objectListCtrl;
	CMyXYGraph m_xyGraphCtrl;
	CControlPanel m_controlPanel;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void update();
};


