// MyTabCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GroundControl.h"
#include "MyTabCtrl.h"


// MyTabCtrl

IMPLEMENT_DYNAMIC(MyTabCtrl, CTabCtrl)

MyTabCtrl::MyTabCtrl()
{

}

MyTabCtrl::~MyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(MyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &MyTabCtrl::OnTcnSelchange)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// MyTabCtrl 메시지 처리기입니다.




void MyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int id = GetCurSel();
	if (id == 0)
	{
		m_controlPanel.ShowWindow(SW_SHOW);
		m_objectListCtrl.ShowWindow(SW_HIDE);
		m_xyGraphCtrl.ShowWindow(SW_HIDE);
	}
	else if (id == 1)
	{
		m_controlPanel.ShowWindow(SW_HIDE);
		m_objectListCtrl.ShowWindow(SW_SHOW);
		m_xyGraphCtrl.ShowWindow(SW_HIDE);
	}
	else if (id == 2)
	{
		m_controlPanel.ShowWindow(SW_HIDE);
		m_objectListCtrl.ShowWindow(SW_HIDE);
		m_xyGraphCtrl.ShowWindow(SW_SHOW);
		
	}
	*pResult = 0;
}


int MyTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InsertItem(0, _T("Control"));
	InsertItem(1, _T("Sensor"));
	InsertItem(2, _T("Graph"));

	RECT rect;
	GetWindowRect(&rect);
	m_objectListCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, CRect(rect.left, rect.top, rect.right - 20, rect.bottom - 40), this, IDC_OBJECT_LIST);
	m_xyGraphCtrl.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | SS_NOTIFY, CRect(rect.left + 150, rect.top, rect.right-50 , rect.bottom-35), this, 1010);
	m_xyGraphCtrl.ShowWindow(SW_HIDE);
	m_objectListCtrl.ShowWindow(SW_HIDE);

	m_controlPanel.Create(NULL, WS_CHILD | WS_VISIBLE, CRect(rect.left , rect.top, rect.right - 20 , rect.bottom - 40), this, 1011);
	


	const list<GroundControl::Node*>& nodelist = GroundControlManager.getNodeList();

	for (list<GroundControl::Node*>::const_iterator iter = nodelist.begin(); iter != nodelist.end(); ++iter)
	{
		m_xyGraphCtrl.addNode((*iter)->name(), (*iter)->pos()[0], (*iter)->pos()[1], (*iter)->Euler()[2]);
	}
	
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}





void MyTabCtrl::update()
{
	m_objectListCtrl.update();
	const list<GroundControl::Node*>& nodelist = GroundControlManager.getNodeList();
	for (list<GroundControl::Node*>::const_iterator iter = nodelist.begin(); iter != nodelist.end(); ++iter)
	{
		m_xyGraphCtrl.updateNode((*iter)->name(), (*iter)->pos()[0], (*iter)->pos()[1], (*iter)->Euler()[2]);
	}
	m_xyGraphCtrl.InvalidateRect(NULL, TRUE);


}
