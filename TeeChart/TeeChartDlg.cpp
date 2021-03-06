
// TeeChartDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TeeChart.h"
#include "TeeChartDlg.h"
#include "afxdialogex.h"

#include "CTChart.h" 
#include "CAxis.h"
#include "CAxes.h"
#include "CScroll.h" 
#include "CSeries.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTeeChartDlg 对话框



CTeeChartDlg::CTeeChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TEECHART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeeChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CTeeChartDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTeeChartDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON1, &CTeeChartDlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTeeChartDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CTeeChartDlg 消息处理程序

BOOL CTeeChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//串口初始化
	m_mscomm.put_CommPort(1);
	m_mscomm.put_Settings(_T("9600,n,8,1"));
	m_mscomm.put_InBufferSize(1024);
	m_mscomm.put_OutBufferSize(1024);
	m_mscomm.put_InputLen(0);
	m_mscomm.put_InputMode(1);
	m_mscomm.put_RThreshold(1);
	CSeries(m_chart.Series(0)).Add(0, _T(""), 1);
	
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	try
	{
		m_pRecordset->Open(_T("SELECT * FROM temp"), theApp.m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle);
	m_list.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(1, _T("电阻"), LVCFMT_LEFT, 50);
	
	int i = 0;
	while (!m_pRecordset->adoEOF)
	{
		m_list.InsertItem(i,_T(""));
		m_list.SetItemText(i,0, (_bstr_t)m_pRecordset->GetCollect(_T("ID")));
		m_list.SetItemText(i, 1, (_bstr_t)m_pRecordset->GetCollect(_T("电阻")));
		i += 1;
		m_pRecordset->MoveNext();
	}
	int count = 0;
	count = m_list.GetItemCount();
	if (count > 0)
	{
		m_list.EnsureVisible(count - 1, FALSE);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeeChartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeeChartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeeChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTeeChartDlg::OnBnClickedButtonOpen()
{
	if (!m_mscomm.get_PortOpen())
	{
		m_mscomm.put_PortOpen(true);
		SetDlgItemText(IDC_BUTTON_OPEN, _T("关闭串口"));
		AfxMessageBox(_T("串口打开成功"));
	}
	else
	{
		m_mscomm.put_PortOpen(false);
		SetDlgItemText(IDC_BUTTON_OPEN, _T("打开串口"));
		AfxMessageBox(_T("串口关闭成功"));
	}// TODO: 在此添加控件通知处理程序代码
}


BEGIN_EVENTSINK_MAP(CTeeChartDlg, CDialog)
	ON_EVENT(CTeeChartDlg, IDC_MSCOMM1, 1, CTeeChartDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CTeeChartDlg::OnCommMscomm1()
{
	VARIANT variant_inp;
	double chartdata;
	COleSafeArray safearray_inp;
	long len, k;
	byte rxdata[1024];
	CString strtemp;
	if (m_mscomm.get_CommEvent() == 2)
	{
		variant_inp = m_mscomm.get_Input();
		safearray_inp = variant_inp;
		len = safearray_inp.GetOneDimSize();
		for (k = 0; k < len; k++)
		{
			safearray_inp.GetElement(&k, rxdata + k);
		}
		for (k = 0; k < len; k++)
		{
			strtemp.Format(_T(" %02x "), *(rxdata + k));//&c字符串 &x 16进制
			
			
			int n = m_list.GetItemCount();
			//保存数据到access数据库
			m_temp = strtemp;
			m_pRecordset->AddNew();
			m_pRecordset->PutCollect("电阻", _variant_t(m_temp));
			m_pRecordset->Update();
			//TeeChart实时显示数据
			chartdata = _ttof(strtemp);
			CSeries(m_chart.Series(0)).Add(chartdata, _T(""), 1);
			CAxes chartaxis = (CAxes)m_chart.get_Axis();
			CAxis chartaxisbottom = (CAxis)chartaxis.get_Bottom();
			chartaxisbottom.Scroll(1, true);
			//更新显示数据
			//m_list.DeleteAllItems();
			while (!m_pRecordset->adoEOF)
			{
				m_list.InsertItem(n, _T(""));
				m_list.SetItemText(n, 0, (_bstr_t)m_pRecordset->GetCollect(_T("ID")));
				m_list.SetItemText(n, 1, (_bstr_t)m_pRecordset->GetCollect(_T("电阻")));
				n += 1;
				m_pRecordset->MoveNext();
			}
			int count = 0;
			count = m_list.GetItemCount();
			if (count > 0)
			{
				m_list.EnsureVisible(count - 1, FALSE);
			}
		}
	}
	UpdateData(false);	
	// TODO: 在此处添加消息处理程序代码
}


void CTeeChartDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTeeChartDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
