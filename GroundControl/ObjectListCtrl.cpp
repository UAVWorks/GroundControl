// ObjectListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GroundControl.h"
#include "ObjectListCtrl.h"


// CObjectListCtrl

IMPLEMENT_DYNAMIC(CObjectListCtrl, CListCtrl)

CObjectListCtrl::CObjectListCtrl() : m_dataID(0)
{

}

CObjectListCtrl::~CObjectListCtrl()
{
}


BEGIN_MESSAGE_MAP(CObjectListCtrl, CListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CObjectListCtrl 메시지 처리기입니다.




int CObjectListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InsertColumn(0, _T("name"), LVCFMT_LEFT, 100);
	InsertColumn(1, _T("position"), LVCFMT_LEFT, 130);
	InsertColumn(2, _T("Rotation"), LVCFMT_LEFT, 130);
	InsertColumn(3, _T("Euler angle"), LVCFMT_LEFT, 130);
	InsertColumn(4, _T("Quarternion"), LVCFMT_LEFT, 160);



	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	const list<GroundControl::Node*>& nodelist = GroundControlManager.getNodeList();




	for (list<GroundControl::Node*>::const_iterator iter = nodelist.begin(); iter != nodelist.end(); iter++)
	{
		const GroundControl::Node* nodeinfo = *iter;
		wstring nodename = wstring(nodeinfo->name().begin(), nodeinfo->name().end());
		InsertObjectData(nodename.c_str(), nodeinfo->pos(), nodeinfo->rot(), nodeinfo->Euler(), nodeinfo->Quarternion());
	}
	
	return 0;
}


int CObjectListCtrl::InsertObjectData(const CString& name, const float* pos, const float* rot, const float* euler, const float* q)
{
	int row = m_dataID * 3;

	InsertItem(row, name);
	InsertItem(row + 1, _T(""));
	InsertItem(row + 2, _T(""));
	
	SetObjectData(m_dataID, pos, rot, euler, q);

	m_dataID++;

	return m_dataID - 1;
}

void CObjectListCtrl::UpdateObjectData(const CString& name, const float* pos, const float* rot, const float* euler, const float* q)
{
	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = name;
		
	int nIndex = FindItem(&info);
	if (nIndex != -1)
	{
		int dataID = nIndex / 3;
		SetObjectData(dataID, pos, rot, euler, q);
	}


}


void CObjectListCtrl::SetObjectData(int idx, const float* pos, const float* rot, const float* euler, const float* q)
{
	CString strText;
	int row = 3 * idx;

	strText.Format(TEXT("%.3f %.3f %.3f"), pos[0], pos[1], pos[2]);
	SetItemText(row, 1, strText);
	strText.Format(TEXT("%.3f %.3f %.3f"), rot[0], rot[1], rot[2]);
	SetItemText(row, 2, strText);
	strText.Format(TEXT("%.3f %.3f %.3f"), rot[3], rot[4], rot[5]);
	SetItemText(row+1, 2, strText);
	strText.Format(TEXT("%.3f %.3f %.3f"), rot[6], rot[7], rot[8]);
	SetItemText(row+2, 2, strText);

	strText.Format(TEXT("%.3f %.3f %.3f"), euler[0], euler[1], euler[2]);
	SetItemText(row, 3, strText);
	
	strText.Format(TEXT("%.3f %.3f %.3f %.3f"), q[0], q[1], q[2], q[3]);
	SetItemText(row, 4, strText);


}


void CObjectListCtrl::update()
{
	const list<GroundControl::Node*>& nodelist = GroundControlManager.getNodeList();
	
	for (list<GroundControl::Node*>::const_iterator iter = nodelist.begin(); iter != nodelist.end(); iter++)
	{
		const GroundControl::Node* nodeinfo = *iter;
		wstring nodename = wstring(nodeinfo->name().begin(), nodeinfo->name().end());		
		UpdateObjectData(nodename.c_str() , nodeinfo->pos(), nodeinfo->rot(), nodeinfo->Euler(), nodeinfo->Quarternion());
	}
}




