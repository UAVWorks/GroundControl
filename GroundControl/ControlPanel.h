#pragma once
#include "MouseControlPanel.h"
#include "ControlInterface.h"
#include "JoystickControl.h"
#include <vector>

using namespace std;
// CControlPanel

class GroundControl::Node;

class CControlPanel : public CStatic
{
	DECLARE_DYNAMIC(CControlPanel)

public:
	CControlPanel();
	virtual ~CControlPanel();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


private:
		
	CButton m_goForwardBtn;
	CButton m_stopBtn;
	CButton m_goBackwardBtn;
	CButton m_goLeftBtn;
	CButton m_goRightBtn;

	CButton m_rnoneBtn;
	CButton m_rmouseBtn;
	CButton m_rjoystickBtn;

	vector<CButton*> m_nodelistBtn;
	vector<GroundControl::Node*> m_nodelist;

	CMouseControlPanel m_mouseControlPanel;
	GroundControl::Node* m_curnode;

	ControlInterface* m_curController;

	CJoystickControl m_joystickControl;
	

public:
	afx_msg void OnGoForwardBtnClicked();
	afx_msg void OnFirstRadioBtnClicked();
	afx_msg void OnSecondRadioBtnClicked();
	afx_msg void OnThirdRadioBtnClicked();
	afx_msg void OnForthRadioBtnClicked();
	afx_msg void OnFifthRadioBtnClicked();
	afx_msg void OnStopBtnClicked();
	afx_msg void OnGoBackwardBtnClicked();
	afx_msg void OnGoLeftBtnClicked();
	afx_msg void OnGoRightBtnClicked();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
		


};


