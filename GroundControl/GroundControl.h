
// GroundControl.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "SystemManager.h"

using namespace std;

// CGroundControlApp:
// �� Ŭ������ ������ ���ؼ��� GroundControl.cpp�� �����Ͻʽÿ�.
//

class CGroundControlApp : public CWinApp
{
public:
	CGroundControlApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CGroundControlApp theApp;
extern GroundControl::SystemManager GroundControlManager;