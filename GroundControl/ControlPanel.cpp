// ControlPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Command.h"
#include "GroundControl.h"
#include "ControlPanel.h"
#include "Message.h"
#include "Node.h"
#include <list>
#include "SystemManager.h"

#define ID_GO_FORWARD_BTN 1020
#define ID_STOP_BTN (ID_GO_FORWARD_BTN+1)
#define ID_GO_BACKWARD_BTN (ID_GO_FORWARD_BTN+2)
#define ID_GO_LEFT_BTN (ID_GO_FORWARD_BTN+3)
#define ID_GO_RIGHT_BTN (ID_GO_FORWARD_BTN+4)
#define ID_NODE_LIST_BTN_BASE 1030
#define ID_MOUSE_CONTROL_PANEL 1031
#define ID_CONTROLLER_BTN_BASE 1040

#define BUTTON_SIZE_X 80 
#define BUTTON_SIZE_Y 80
// CControlPanel

IMPLEMENT_DYNAMIC(CControlPanel, CStatic)

CControlPanel::CControlPanel() : m_curnode(NULL)
{

}

CControlPanel::~CControlPanel()
{
	int size = m_nodelistBtn.size();
	for (int i = 0; i < size; i++)
	{
		delete m_nodelistBtn[i];
	}
}


BEGIN_MESSAGE_MAP(CControlPanel, CStatic)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_GO_FORWARD_BTN, &CControlPanel::OnGoForwardBtnClicked)
	ON_BN_CLICKED(ID_STOP_BTN, &CControlPanel::OnStopBtnClicked)
	ON_BN_CLICKED(ID_GO_BACKWARD_BTN, &CControlPanel::OnGoBackwardBtnClicked)
	ON_BN_CLICKED(ID_GO_LEFT_BTN, &CControlPanel::OnGoLeftBtnClicked)
	ON_BN_CLICKED(ID_GO_RIGHT_BTN, &CControlPanel::OnGoRightBtnClicked)
	ON_BN_CLICKED(ID_NODE_LIST_BTN_BASE, &CControlPanel::OnFirstRadioBtnClicked)
	ON_BN_CLICKED(ID_NODE_LIST_BTN_BASE + 1, &CControlPanel::OnSecondRadioBtnClicked)
	ON_BN_CLICKED(ID_CONTROLLER_BTN_BASE, &CControlPanel::OnThirdRadioBtnClicked)
	ON_BN_CLICKED(ID_CONTROLLER_BTN_BASE + 1, &CControlPanel::OnForthRadioBtnClicked)
	ON_BN_CLICKED(ID_CONTROLLER_BTN_BASE + 2, &CControlPanel::OnFifthRadioBtnClicked)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CControlPanel 메시지 처리기입니다.




int CControlPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rect;
	GetWindowRect(&rect);

	list<GroundControl::Node*>& nodelist = GroundControlManager.getNodeList();

	int idx = 0;

	for (list<GroundControl::Node*>::iterator iter = nodelist.begin(); iter != nodelist.end(); ++iter)
	{
		GroundControl::Node* node = *iter;
		m_nodelist.push_back(node);
		CButton* rButton = new CButton();
			
		wstring wstr = wstring(node->name().begin(), node->name().end());

		rButton->Create(wstr.c_str(), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, CRect(rect.left + 20, rect.top + 20 * (idx + 1), rect.left + 20 + 100, rect.top + 20 * (idx + 1) + 20), this, ID_NODE_LIST_BTN_BASE+idx);
		if (idx == 0)
		{
			rButton->SetCheck(BST_CHECKED);
			m_curnode = node;

		}
		else
		{
			rButton->SetCheck(BST_UNCHECKED);
		}
		m_nodelistBtn.push_back(rButton);
		idx++;
	}

	m_rnoneBtn.Create(_T("None"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, CRect(rect.left + 160, rect.top + 20, rect.left + 160 + 100, rect.top + 20 + 20), this, ID_CONTROLLER_BTN_BASE);
	m_rmouseBtn.Create(_T("Mouse"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, CRect(rect.left + 160, rect.top + 40, rect.left + 160 + 100, rect.top + 40 + 20), this, ID_CONTROLLER_BTN_BASE+1);
	m_rjoystickBtn.Create(_T("Joystick"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, CRect(rect.left + 160, rect.top + 60, rect.left + 160 + 100, rect.top + 60 + 20), this, ID_CONTROLLER_BTN_BASE+2);
	m_rnoneBtn.SetCheck(BST_UNCHECKED);
	m_rmouseBtn.SetCheck(BST_CHECKED);
	m_rjoystickBtn.SetCheck(BST_UNCHECKED);

	m_goForwardBtn.Create(_T("전진"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(rect.left + 110, rect.top + 100, rect.left + 110 + BUTTON_SIZE_X, rect.top + 100 + BUTTON_SIZE_Y), this, ID_GO_FORWARD_BTN);
	m_stopBtn.Create(_T("정지"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(rect.left + 110, rect.top + 200, rect.left + 110 + BUTTON_SIZE_X, rect.top + 200 + BUTTON_SIZE_Y), this, ID_STOP_BTN);
	m_goBackwardBtn.Create(_T("후진"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(rect.left + 110, rect.top + 300, rect.left + 110 + BUTTON_SIZE_X, rect.top + 300 + BUTTON_SIZE_Y), this, ID_GO_BACKWARD_BTN);
	m_goLeftBtn.Create(_T("좌회전"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(rect.left + 10, rect.top + 200, rect.left + 10 + BUTTON_SIZE_X, rect.top + 200 + BUTTON_SIZE_Y), this, ID_GO_LEFT_BTN);
	m_goRightBtn.Create(_T("우회전"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(rect.left + 210, rect.top + 200, rect.left + 210 + BUTTON_SIZE_X, rect.top +200 + BUTTON_SIZE_Y), this, ID_GO_RIGHT_BTN);

	m_mouseControlPanel.Create( NULL, NULL, WS_CHILD | WS_VISIBLE | SS_NOTIFY | WS_BORDER, CRect(rect.left + 310, rect.top + 0, rect.left + 310 + 400, rect.top + 0 + 400), this, ID_MOUSE_CONTROL_PANEL);
	
	SetTimer(100, 30, NULL);

	m_curController = &m_mouseControlPanel;
	return 0;
}


void CControlPanel::OnGoForwardBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_GO_FORWARD, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}

void CControlPanel::OnStopBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_STOP, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}

void CControlPanel::OnGoBackwardBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_GO_BACKWARD, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}

void CControlPanel::OnGoLeftBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_GO_LEFT, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}

void CControlPanel::OnGoRightBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	if (m_curnode)
	{
		GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_GO_RIGHT, NULL, 0);
		m_curnode->sendCommand(&cmd);
	}
}

void CControlPanel::OnFirstRadioBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	int size = m_nodelistBtn.size();

	if (size< 1)
		return;

	for (int i = 0; i < size; i++)
		m_nodelistBtn[i]->SetCheck(BST_UNCHECKED);

	m_nodelistBtn[0]->SetCheck(BST_CHECKED);
	m_curnode = m_nodelist[0];


}

void CControlPanel::OnSecondRadioBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	int size = m_nodelistBtn.size();

	if (size< 2)
		return;

	for (int i = 0; i < size; i++)
		m_nodelistBtn[i]->SetCheck(BST_UNCHECKED);

	m_nodelistBtn[1]->SetCheck(BST_CHECKED);
	m_curnode = m_nodelist[1];
}

void CControlPanel::OnThirdRadioBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	m_rnoneBtn.SetCheck(BST_CHECKED);
	m_rmouseBtn.SetCheck(BST_UNCHECKED);
	m_rjoystickBtn.SetCheck(BST_UNCHECKED);
	m_curController = NULL; // None
}

void CControlPanel::OnForthRadioBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	m_rnoneBtn.SetCheck(BST_UNCHECKED);
	m_rmouseBtn.SetCheck(BST_CHECKED);
	m_rjoystickBtn.SetCheck(BST_UNCHECKED);
	m_curController = &m_mouseControlPanel; // mouse
}

void CControlPanel::OnFifthRadioBtnClicked()
{
	//AfxMessageBox(_T("clicked"));

	m_rnoneBtn.SetCheck(BST_UNCHECKED);
	m_rmouseBtn.SetCheck(BST_UNCHECKED);
	m_rjoystickBtn.SetCheck(BST_CHECKED);
	m_curController = &m_joystickControl; //joystick
}

void CControlPanel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_curController)
	{
		short linear = 0, angular = 0;
		m_curController->update();
		m_curController->getCommand(linear, angular);
			
		byte data[4];
		memcpy(data, &linear, 2);
		memcpy(data + 2, &angular, 2);

		if (m_curnode)
		{
			GroundControl::GeneralMessage cmd(TURTLEBOT_MSG_MOVE, data, 4);
			m_curnode->sendCommand(&cmd);
		}
	}
	CStatic::OnTimer(nIDEvent);
}
