// MouseControlPanel.cpp : 구현 파일입니다.
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
#define MOUSE_STEP 20

CPoint centerPoint(MOUSE_PANEL_WIDTH/2, MOUSE_PANEL_HEIGHT/2);
CPoint curPoint(centerPoint); // Mouse current Point (moving point)
CPoint desPoint(centerPoint); // Mouse desired Point
CPoint errPoint(0,0);

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
END_MESSAGE_MAP()



// CMouseControlPanel 메시지 처리기입니다.




void CMouseControlPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonDown(nFlags, point);

	mouseFlag = true;
}


void CMouseControlPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonUp(nFlags, point);

	mouseFlag = false;

}


void CMouseControlPanel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	   
	CWnd::OnTimer(nIDEvent);

	if (mouseFlag == true)
	{
		desPoint = movingPoint;
		errPoint = (desPoint - curPoint);
		float abs_errPoint = sqrt(pow(errPoint.x, 2) + pow(errPoint.y, 2));
		if (abs_errPoint <= MOUSE_STEP)
		{
			curPoint.x = desPoint.x;
			curPoint.y = desPoint.y;
		}
		else
		{
			curPoint.x = curPoint.x + ((errPoint.x / abs_errPoint) * MOUSE_STEP);
			curPoint.y = curPoint.y + ((errPoint.y / abs_errPoint) * MOUSE_STEP);
		}
	}

	if (mouseFlag != true && curPoint != centerPoint)
	{
		desPoint = centerPoint;
		errPoint = (desPoint - curPoint);
		float abs_errPoint = sqrt(pow(errPoint.x, 2) + pow(errPoint.y, 2));
		if (abs_errPoint <= MOUSE_STEP)
		{
			curPoint.x = desPoint.x;
			curPoint.y = desPoint.y;
		}
		else
		{
			curPoint.x = curPoint.x + ((errPoint.x / abs_errPoint) * MOUSE_STEP);
			curPoint.y = curPoint.y + ((errPoint.y / abs_errPoint) * MOUSE_STEP);
		}
	}
	
	byte* Cdata = new byte[4];
	int LCommand = -5 * curPoint.y + 1000;
	int ACommand = 5 * curPoint.x - 1000;
	int LAC = LCommand * 1000 + ACommand;

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_MOVE, Cdata, 0);
		m_curnode->sendCommand(&cmd);
	}
	
	Invalidate();
}



int CMouseControlPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(2001, 50, NULL);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

/*
void int2Byte(int value, byte data[], int idx)
{
	data[idx] = (byte)(value >> 24);
	data[++idx] = (byte)(value >> 16);
	data[++idx] = (byte)(value >> 8);
	data[++idx] = (byte)value;
}*/