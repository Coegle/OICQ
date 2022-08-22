
// OICQDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OICQ.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "SocketC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "ClistDlg.h"


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




// COICQDlg 对话框
CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OICQ_DIALOG, pParent)
	, edit_name(_T("test1"))
	, edit_pass(_T("123456"))
	, m_caption(_T(""))
	, edit_ipaddr(_T("127.0.0.1"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBTN_LOGIN, btn_login);
	DDX_Control(pDX, IDC_CBTN_REG, btn_reg);
	DDX_Text(pDX, IDC_EDIT_NAME, edit_name);
	DDV_MaxChars(pDX, edit_name, 15);
	DDX_Text(pDX, IDC_EDIT_PASS, edit_pass);
	DDV_MaxChars(pDX, edit_pass, 15);
	DDX_Text(pDX, IDC_EDIT_IP, edit_ipaddr);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CBTN_LOGIN, &CLoginDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_CBTN_REG, &CLoginDlg::OnBnClickedRegist)
END_MESSAGE_MAP()


// COICQDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	theApp.sock.logindlg = this;
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoginDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//	登录
void CLoginDlg::OnBnClickedLogin()
{
	CString tmp_addr = theApp.ipaddress;
	UpdateData();
	if (edit_name.GetLength() == 0) {
		AfxMessageBox(_T("用户名不能为空！"));
		return;
	}
	if (edit_pass.GetLength() == 0) {
		AfxMessageBox(_T("密码不能为空！"));
		return;
	}

	if (edit_ipaddr.GetLength() != 0) {
		tmp_addr = edit_ipaddr;
	}

	if (theApp.sock.Create() && theApp.sock.Connect(tmp_addr, theApp.Port)) {
		theApp.sock.SendControlMessage(edit_name, edit_pass, TYPE_LOGIN);
		theApp.username = edit_name;
	}
	else {
		AfxMessageBox(_T("连接失败！"));
		theApp.sock.CloseSocket();
	}
}

//	注册
void CLoginDlg::OnBnClickedRegist()
{
	UpdateData();
	CString tmp_addr = theApp.ipaddress;
	if (edit_name.GetLength() == 0) {
		AfxMessageBox(_T("用户名不能为空！"));
		return;
	}
	if (edit_pass.GetLength() == 0) {
		AfxMessageBox(_T("密码不能为空！"));
		return;
	}
	
	if (edit_ipaddr.GetLength() != 0) {
		tmp_addr = edit_ipaddr;
	}

	if (theApp.sock.Create() && theApp.sock.Connect(tmp_addr, theApp.Port)) {
		theApp.sock.SendControlMessage(edit_name, edit_pass, TYPE_REGIST);
		theApp.username = edit_name;
	}
	else {
		AfxMessageBox(_T("连接失败！"));
		theApp.sock.CloseSocket();
	}
}


void CLoginDlg::OnOK()
{
	CDialogEx::OnOK();
}
