// MouseControlPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GroundControl.h"
#include "MouseControlPanel.h"
#include "Command.h"
#include "MemDC.h"
#include "GroundControl.h"
#include "ControlPanel.h"
#include "Message.h"
#include "Node.h"
#include <list>
#include "SystemManager.h"
#include <afxwin.h>
#include <afxdialogex.h>

#define MOUSE_PANEL_WIDTH 400
#define MOUSE_PANEL_HEIGHT 400
#define MAX_DRAW_POINT 50
#define MOUSE_STEP 10

CPoint centerPoint(MOUSE_PANEL_WIDTH/2, MOUSE_PANEL_HEIGHT/2);
CPoint curPoint(centerPoint); // Mouse current Point (moving point)
CPoint desPoint(centerPoint); // Mouse desired Point
CPoint errPoint(0,0);

bool mouseFlag = false;

// CMouseControlPanel

IMPLEMENT_DYNAMIC(CMouseControlPanel, CWnd)

CMouseControlPanel::CMouseControlPanel() : m_rateLimiter(MOUSE_STEP, Eigen::Vector2f(centerPoint.x, centerPoint.y))
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
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMouseControlPanel �޽��� ó�����Դϴ�.




void CMouseControlPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CStatic::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CCustomMemDC memdc(&dc);

	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	CPen* oldpen = memdc.SelectObject(&pen);

	memdc.MoveTo(CPoint(MOUSE_PANEL_WIDTH / 2, 0));
	memdc.LineTo(CPoint(MOUSE_PANEL_WIDTH / 2, MOUSE_PANEL_HEIGHT));
	memdc.MoveTo(CPoint(0, MOUSE_PANEL_HEIGHT / 2));
	memdc.LineTo(CPoint(MOUSE_PANEL_WIDTH, MOUSE_PANEL_HEIGHT / 2));
	/***********************************************************************/

	/*	CPen pen1;
		pen1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

		CPen* oldpen1 = dc.SelectObject(&pen1);

		if (mouseFlag == true)
		{
		dc.MoveTo(CPoint(centerPoint));
		dc.LineTo(CPoint(movingPoint.x, movingPoint.y));

		}
		*/

	CPen pen2;
	pen2.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	CPen* oldpen2 = memdc.SelectObject(&pen2);
	
	if (mouseFlag == true)
	{
		memdc.Ellipse(movingPoint.x - 5, movingPoint.y - 5, movingPoint.x + 5, movingPoint.y + 5);
	}

	CPen pen3;
	pen3.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* oldpen3 = memdc.SelectObject(&pen3);
	memdc.Ellipse(curPoint.x-5, curPoint.y-5, curPoint.x + 5, curPoint.y + 5);

}


void CMouseControlPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnLButtonDown(nFlags, point);

	mouseFlag = true;
}


void CMouseControlPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnMouseMove(nFlags, point);

	movingPoint = point;

//	CString text;
//	text.Format(_T("x:%d, y:%d\n L:%d, A:%d"), point.x, point.y, LCommand, ACommand);
	
/*	if (mouseFlag == true)
	{
		AfxMessageBox(text);
	}

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_MOVE, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}*/
}


void CMouseControlPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnLButtonUp(nFlags, point);

	mouseFlag = false;

}


int CMouseControlPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;



	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}


void CMouseControlPanel::update()
{

	Eigen::Vector2f desired(movingPoint.x, movingPoint.y);

	if (!mouseFlag)
	{
		desired[0] = centerPoint.x;
		desired[1] = centerPoint.y;
	}

	Eigen::Vector2f val = m_rateLimiter(desired);

	curPoint.x = val[0];
	curPoint.y = val[1];
	Invalidate();

}

void CMouseControlPanel::getCommand(short& linear, short& angular)
{
	linear = (-5 * curPoint.y + 1000)/2;
	angular = -(5 * curPoint.x - 1000);
}