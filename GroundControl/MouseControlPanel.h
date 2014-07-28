#pragma once


// CMouseControlPanel

class CMouseControlPanel : public CWnd
{
	DECLARE_DYNAMIC(CMouseControlPanel)

public:
	CMouseControlPanel();
	virtual ~CMouseControlPanel();

protected:
	DECLARE_MESSAGE_MAP()

private:
	GroundControl::Node* m_curnode;
	CPoint movingPoint;

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

};


