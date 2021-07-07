#include "pch.h"
#include "framework.h"
#include "BBungGridExample.h"
#include "BBungGridExampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CBBungGridExampleDlg::CBBungGridExampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BBUNGGRIDEXAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBBungGridExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_ctlPropGrid);
}

BEGIN_MESSAGE_MAP(CBBungGridExampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



BOOL CBBungGridExampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	

	Init();		// <--- Init BBungGrid

	return TRUE; 
}

void CBBungGridExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CBBungGridExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CBBungGridExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBBungGridExampleDlg::Init()
{
	m_strDataName = _T("1BE99CCD");
	m_nX = 10;
	m_nY = 20;
	m_dTheta = 45.9;
	m_nType = 1;
	m_clrColor = RGB(255, 192, 192);

	m_font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	CBBungGrid::stProperty GRID_STUFF[] =
	{
		P_ROOT(_T("TEXT:Cell Data;")),
			P_SUBROOT(_T("TEXT:Cell Data;")),
				P_VAR(m_strDataName, _T("TEXT:Data Name;ENABLE:FALSE;")),
				P_VAR(m_nX, _T("TEXT:X;")),
				P_VAR(m_nY, _T("TEXT:Y;")),
				P_VAR(m_dTheta, _T("TEXT:Theta;")),
				P_VAR(m_nType, _T("TEXT:Type;TYPE:1;ARR:Type1,Type2,Type3;")),
			P_SUBROOT_END(),
			P_SUBROOT(_T("TEXT:Other;")),
				P_VAR(m_clrColor, _T("TEXT:Color;TYPE:2;")),
			P_SUBROOT_END(),
		P_ROOT_END(),
	};
	int nSize = sizeof(GRID_STUFF) / sizeof(CBBungGrid::stProperty);

	m_ctlPropGrid.Initial(&m_font);
	m_ctlPropGrid.AppendProperty(GRID_STUFF, nSize);
}


void CBBungGridExampleDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_font.DeleteObject();
}
