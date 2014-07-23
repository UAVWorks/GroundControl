// MyXYGraph.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MemDC.h"
#include "GroundControl.h"
#include "MyXYGraph.h"


// CMyXYGraph

#define GRAPH_WIDTH 500
#define GRAPH_HEIGHT 500
#define MAX_DRAW_POINT 50


CPoint startPoint(50, 5);



DWORD NODE_COLOR[] = { RGB(255,0,0), RGB(0,255,0), RGB(0,0,255), AFX_IDC_COLOR_CYAN, AFX_IDC_COLOR_YELLOW, AFX_IDC_COLOR_MAGENTA };

IMPLEMENT_DYNAMIC(CMyXYGraph, CWnd)

CMyXYGraph::CMyXYGraph() : m_xStep(1), m_yStep(1), m_centerX(0), m_centerY(0)
{

}

CMyXYGraph::~CMyXYGraph()
{
}


BEGIN_MESSAGE_MAP(CMyXYGraph, CWnd)
	ON_WM_PAINT()

END_MESSAGE_MAP()



// CMyXYGraph 메시지 처리기입니다.


void CMyXYGraph::drawBackgroundLine(CPaintDC& dc)
{

	CCustomMemDC memdc(&dc);

	RECT rect;

	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	CPen* oldpen = memdc.SelectObject(&pen);



	
	memdc.MoveTo(startPoint);
	memdc.LineTo(startPoint + CPoint(GRAPH_WIDTH, 0));
	memdc.LineTo(startPoint + CPoint(GRAPH_WIDTH, GRAPH_HEIGHT));
	memdc.LineTo(startPoint + CPoint(0, GRAPH_HEIGHT));
	memdc.LineTo(startPoint);


	CPen dashpen(PS_DOT, 1, RGB(100, 100, 100));
	memdc.SelectObject(&dashpen);


	/// vertical line... 

	float xLabel1 = -5.f * m_xStep;
	float xLabel2 = m_xStep;


	CString str;
	str.Format(TEXT("%.2f"), xLabel1);
	CPoint pt = startPoint + CPoint(0, GRAPH_HEIGHT);
	memdc.TextOutW(pt.x - 15, pt.y + 5, str);
	xLabel1 += m_xStep;

	pt = startPoint + CPoint(GRAPH_WIDTH / 2, GRAPH_HEIGHT);
	memdc.TextOutW(pt.x - 15, pt.y + 5, _T("0.00"));


	for (int i = 0; i < 4; i++)
	{
		CPoint start1 = startPoint + CPoint((i + 1)*GRAPH_WIDTH / 10, 0);
		CPoint start2 = start1 + CPoint(GRAPH_WIDTH / 2, 0);
		CPoint end1 = start1 + CPoint(0, GRAPH_HEIGHT);
		CPoint end2 = start2 + CPoint(0, GRAPH_HEIGHT);

		memdc.MoveTo(start1);
		memdc.LineTo(end1);
		memdc.MoveTo(start2);
		memdc.LineTo(end2);

		str.Format(TEXT("%.2f"), xLabel1);
		memdc.TextOutW(end1.x - 15, end1.y + 5, str);
		str.Format(TEXT("%.2f"), xLabel2);
		memdc.TextOutW(end2.x - 15, end2.y + 5, str);
		xLabel1 += m_xStep;
		xLabel2 += m_xStep;
	}

	str.Format(TEXT("%.2f"), xLabel2);
	pt = startPoint + CPoint(GRAPH_WIDTH, GRAPH_HEIGHT);
	memdc.TextOutW(pt.x - 15, pt.y + 5, str);




	// horizontal line.. 


	float yLabel1 = 5 * m_yStep;
	float yLabel2 = -m_yStep;


	str.Format(TEXT("%.2f"), yLabel1);
	pt = startPoint;
	memdc.TextOutW(pt.x - 35, pt.y - 7, str);
	yLabel1 -= m_yStep;


	pt = startPoint + CPoint(0, GRAPH_HEIGHT / 2);
	memdc.TextOutW(pt.x - 35, pt.y - 7, _T("0.00"));


	for (int i = 0; i < 4; i++)
	{
		CPoint start1 = startPoint + CPoint(0, (i + 1)*GRAPH_HEIGHT / 10);
		CPoint start2 = start1 + CPoint(0, GRAPH_HEIGHT / 2);
		CPoint end1 = start1 + CPoint(GRAPH_WIDTH, 0);
		CPoint end2 = start2 + CPoint(GRAPH_WIDTH, 0);

		memdc.MoveTo(start1);
		memdc.LineTo(end1);
		memdc.MoveTo(start2);
		memdc.LineTo(end2);

		CString str;
		str.Format(TEXT("%.2f"), yLabel1);
		memdc.TextOutW(start1.x - 35, start1.y - 7, str);
		str.Format(TEXT("%.2f"), yLabel2);
		memdc.TextOutW(start2.x - 35, start2.y - 7, str);

		yLabel1 -= m_yStep;
		yLabel2 -= m_yStep;
	}

	str.Format(TEXT("%.2f"), yLabel2);
	pt = startPoint + CPoint(0, GRAPH_HEIGHT);
	memdc.TextOutW(pt.x - 35, pt.y - 7, str);


	memdc.SelectObject(oldpen);

	memdc.MoveTo(startPoint + CPoint(GRAPH_WIDTH / 2, 0));
	memdc.LineTo(startPoint + CPoint(GRAPH_WIDTH / 2, GRAPH_HEIGHT));

	memdc.MoveTo(startPoint + CPoint(0, GRAPH_HEIGHT / 2));
	memdc.LineTo(startPoint + CPoint(GRAPH_WIDTH, GRAPH_HEIGHT / 2));


	memdc.SelectObject(oldpen);

}


void CMyXYGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.

	drawBackgroundLine(dc);
	drawNodes(dc);
}


void CMyXYGraph::drawNodes(CPaintDC& dc)
{
	int colorIndex = 0;
	for (list<DrawInfo>::iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
	{

		CPen pen;
		pen.CreatePen(PS_SOLID, 1, NODE_COLOR[colorIndex] );
		dc.SelectObject(&pen);

		DrawInfo& node = *iter;
		list<DrawInfo::PosInfo>::iterator posIter = node.posInfoes.begin();
		dc.MoveTo(posIter->pt);
		posIter++;
		for (; posIter != node.posInfoes.end(); ++posIter)
		{
			dc.LineTo(posIter->pt);
		}
		colorIndex++;
	}

}

POINT CMyXYGraph::getPixelPos(float x, float y)
{
	int pixelPerMeterX = GRAPH_WIDTH / 10 / m_xStep;
	int pixelPerMeterY = GRAPH_HEIGHT / 10 / m_yStep;

	POINT pt;
	pt.x = GRAPH_WIDTH/2 + (x - m_centerX) * pixelPerMeterX + startPoint.x;
	pt.y = GRAPH_HEIGHT / 2 - (y - m_centerY) * pixelPerMeterY + startPoint.y;

	return pt;
}

void CMyXYGraph::addNode(const string& name, float x, float y, float yaw)
{
	DrawInfo node;
	node.name = name;
	DrawInfo::PosInfo posinfo;
	posinfo.pt = getPixelPos(x, y);
	posinfo.yaw = yaw;
	
	node.posInfoes.push_back(posinfo);

	m_nodes.push_back(node);
}


void CMyXYGraph::updateNode(const string& name, float x, float y, float yaw)
{
	for (list<DrawInfo>::iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
	{
		if (iter->name == name)
		{
			DrawInfo::PosInfo posinfo;
			posinfo.pt = getPixelPos(x, y);
			posinfo.yaw = yaw;

			iter->posInfoes.push_back(posinfo);

			if (iter->posInfoes.size() >= MAX_DRAW_POINT)
			{
				iter->posInfoes.pop_front();
			}
			break;
		}
	}
}


