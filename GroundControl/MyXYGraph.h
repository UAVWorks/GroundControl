#pragma once

#include <queue> 
#include <string>

using namespace std;

// CMyXYGraph

class CMyXYGraph : public CWnd
{
	DECLARE_DYNAMIC(CMyXYGraph)



public:
	CMyXYGraph();
	virtual ~CMyXYGraph();

			
	struct DrawInfo
	{
		struct PosInfo
		{
			POINT pt;
			float yaw;
		};
		string name;
			
		list<PosInfo> posInfoes;
	};

		
	void addNode(const string& name, float x, float y, float yaw);
	void updateNode(const string& name, float x, float y, float yaw);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	
	void drawBackgroundLine(CPaintDC& dc);
	void drawNodes(CPaintDC& dc);
	POINT getPixelPos(float x, float y);
	float m_xStep;
	float m_yStep;

	float m_centerX;
	float m_centerY;

	list<DrawInfo> m_nodes;
public:

};


