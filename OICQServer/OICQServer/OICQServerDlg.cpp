
// OICQServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OICQServerDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <afxlistctrl.h>
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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


// COICQServerDlg 对话框



COICQServerDlg::COICQServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OICQSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

COICQServerDlg::~COICQServerDlg()
{
	//while (theApp.msgsock.size() != 0) {
	//	auto it = theApp.msgsock.begin();
	//	delete it->second;
	//	theApp.msgsock.erase(it);
	//}
}

void COICQServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST_MSG, list_msg);
	DDX_Control(pDX, IDC_LIST_EVE, list_eve);
	DDX_Control(pDX, IDC_LIST_ONLINE, list_online);
	DDX_Control(pDX, IDC_LIST_MSG, list_msg);
	DDX_Control(pDX, IDC_EDIT_IP, edit_ip);
}

BEGIN_MESSAGE_MAP(COICQServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &COICQServerDlg::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON1, &COICQServerDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &COICQServerDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON3, &COICQServerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// COICQServerDlg 消息处理程序

BOOL COICQServerDlg::OnInitDialog()
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
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	wchar_t name[USER_SIZE] = { 0 };
	wchar_t pass[PASS_SIZE] = { 0 };

	std::ifstream reader(theApp.PATH, std::ios::binary);
	while (!reader.eof()) {
		User tmp;
		
		reader.read((char*)&name, USER_SIZE*2);
		reader.read((char*)&pass, PASS_SIZE*2);
		if (reader.fail()) {
			break;
		}
		tmp.password = pass;
		tmp.username = name;
		theApp.Registed_User[tmp.username] = tmp;
	}
	reader.close();

	list_msg.InsertColumn(0, _T("To"), LVCFMT_CENTER, 50);
	list_msg.InsertColumn(1, _T("From"), LVCFMT_CENTER, 50);
	list_msg.InsertColumn(2, _T("Content"), LVCFMT_CENTER, 300);

	SetWindowText(CString("OICQ 服务器"));

	theApp.sockL.Init(theApp.Port, this);

	char szHostName[128] = { 0 };
	gethostname(szHostName, 128);									//获取服务器名称
	hostent* pHostent = gethostbyname(szHostName);
	char* szIP = inet_ntoa(*(in_addr*)&pHostent->h_addr_list[2]);	//获取IP地址
	USES_CONVERSION;
	CString cstr_ip = A2T(szIP);
	CString cstr_port;
	cstr_port.Format(_T("%d"), theApp.Port);
	edit_ip.SetWindowTextW(cstr_ip + CString(":") + cstr_port);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COICQServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COICQServerDlg::OnPaint()
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
HCURSOR COICQServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COICQServerDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

//void COICQServerDlg::OnBnClickedButton1() // 发送消息
//{
//	std::map<CString, SocketC*>::iterator it = theApp.msgsock.find(_T("test1"));
//	if (it != theApp.msgsock.end()) {
//		Mesg mesg;
//		mesg.length = sizeof("hello");
//		memcpy(mesg.content, _T("hello!"), sizeof("hello!")*2);
//		memcpy(mesg.from, _T("aa"), sizeof("aa")*2);
//		memcpy(mesg.to, _T("test1"), sizeof("test1")*2);
//		it->second->SendTextData(mesg);
//	}
//}
//
//
//void COICQServerDlg::OnBnClickedButton2() //上线
//{
//	std::map<CString, SocketC*>::iterator it = theApp.msgsock.find(_T("test1"));
//	if (it != theApp.msgsock.end()) {
//		it->second->SendNotification(TYPE_ONLINE, CString("test2"));
//	}
//}
//
//
//void COICQServerDlg::OnBnClickedButton3() //下线
//{
//	std::map<CString, SocketC*>::iterator it = theApp.msgsock.find(_T("test1"));
//	if (it != theApp.msgsock.end()) {
//		it->second->SendNotification(TYPE_OFFLINE, CString("test2"));
//	}
//}
