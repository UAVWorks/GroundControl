#pragma once
#include "RateLimiter.h"
#include "ControlInterface.h"


// CMouseControlPanel

class CMouseControlPanel : public CWnd , public ControlInterface
{
	DECLARE_DYNAMIC(CMouseControlPanel)

public:
	CMouseControlPanel();
	virtual ~CMouseControlPanel();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CPoint movingPoint;
	RateLimiter m_rateLimiter;

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void update();
	virtual void getCommand(short& linear, short& angular);

	

};


