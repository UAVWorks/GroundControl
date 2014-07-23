
// GroundControlDlg.h : ��� ����
//
#pragma once
#include "MyTabCtrl.h"
#include "ViconMotionCapture.h"

#include "afxcmn.h"


// CGroundControlDlg ��ȭ ����
class CGroundControlDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CGroundControlDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GROUNDCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	MyTabCtrl m_tabCtrl;
	bool m_bfilesave;
	FILE* m_file;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSave();
	afx_msg void OnDestroy();

};
