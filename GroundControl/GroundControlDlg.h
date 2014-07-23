
// GroundControlDlg.h : 헤더 파일
//
#pragma once
#include "MyTabCtrl.h"
#include "ViconMotionCapture.h"

#include "afxcmn.h"


// CGroundControlDlg 대화 상자
class CGroundControlDlg : public CDialogEx
{
// 생성입니다.
public:
	CGroundControlDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GROUNDCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
