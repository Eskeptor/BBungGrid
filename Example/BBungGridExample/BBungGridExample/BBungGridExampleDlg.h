#pragma once
#include "BBungGrid.h"

class CBBungGridExampleDlg : public CDialogEx
{
// 생성입니다.
public:
	CBBungGridExampleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BBUNGGRIDEXAMPLE_DIALOG };
#endif

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
	CBBungGrid m_ctlPropGrid;
	CFont m_font;

	int m_nX;
	int m_nY;
	double m_dTheta;
	int m_nType;
	CString m_strDataName;
	CString m_strID;
	COLORREF m_clrColor;

	void Init();
	afx_msg void OnDestroy();
};
