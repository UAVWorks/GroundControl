// MouseControlPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GroundControl.h"
#include "MouseControlPanel.h"
#include "Command.h"
#include "GroundControl.h"
#include "ControlPanel.h"
#include "Message.h"
#include "MemDC.h"
#include "Node.h"
#include <list>
#include "SystemManager.h"
#include <afxwin.h>
#include <afxdialogex.h>

#define MOUSE_PANEL_WIDTH 400
#define MOUSE_PANEL_HEIGHT 400
#define MAX_DRAW_POINT 50

CPoint centerPoint(MOUSE_PANEL_WIDTH/2, MOUSE_PANEL_HEIGHT/2);

bool mouseFlag = false;

// CMouseControlPanel

IMPLEMENT_DYNAMIC(CMouseControlPanel, CWnd)

CMouseControlPanel::CMouseControlPanel()
{

}

CMouseControlPanel::~CMouseControlPanel()
{
}


BEGIN_MESSAGE_MAP(CMouseControlPanel, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMouseControlPanel �޽��� ó�����Դϴ�.




void CMouseControlPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CStatic::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CCustomMemDC memdc(&dc);
	RECT rect;
	GetClientRect(&rect);



	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	CPen* oldpen = memdc.SelectObject(&pen);
	
	memdc.MoveTo(CPoint(MOUSE_PANEL_WIDTH / 2, 0));
	memdc.LineTo(CPoint(MOUSE_PANEL_WIDTH / 2, MOUSE_PANEL_HEIGHT));
	memdc.MoveTo(CPoint(0, MOUSE_PANEL_HEIGHT / 2));
	memdc.LineTo(CPoint(MOUSE_PANEL_WIDTH, MOUSE_PANEL_HEIGHT / 2));
	/***********************************************************************/

	memdc.draw

	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	CPen* oldpen1 = memdc.SelectObject(&pen1);
	
	if (mouseFlag == true)
	{
	
		memdc.MoveTo(CPoint(centerPoint));
		memdc.LineTo(CPoint(movingPoint.x, movingPoint.y));

	}
	
/*	dc.SelectObject(&oldpen1);
	dc.DeleteDC();
	pen1.DeleteObject();*/

}


void CMouseControlPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnLButtonDown(nFlags, point);

	mouseFlag = true;
	

/*	CString text;
	text.Format(_T("x:%d, y:%d"), point.x, point.y);
	AfxMessageBox(text);*/

/*	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_MOVE, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}*/


}


void CMouseControlPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnMouseMove(nFlags, point);

	int LCommand = -5 * point.y + 1000;
	int ACommand = 5 * point.x - 1000;

	movingPoint = point;

	CString text;
	text.Format(_T("x:%d, y:%d\n L:%d, A:%d"), point.x, point.y, LCommand, ACommand);
	
	if (mouseFlag == true)
	{
		//AfxMessageBox(text);
	}

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_MOVE, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}


void CMouseControlPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnLButtonUp(nFlags, point);

	//KillTimer(2001);

	mouseFlag = false;

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_MOVE, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}


void CMouseControlPanel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnTimer(nIDEvent);
	RECT rect;
//	GetWindowRect(&rect);
//	InvalidateRect(&rect);

	Invalidate();
}


int CMouseControlPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(2001, 100, NULL);

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


BOOL CMouseControlPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CWnd::OnEraseBkgnd(pDC);
	//return TRUE;
}
