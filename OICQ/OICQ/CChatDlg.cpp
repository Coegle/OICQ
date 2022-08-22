// CChatDlg.cpp: 实现文件
//

#include "pch.h"
#include "OICQ.h"
#include "CChatDlg.h"
#include "afxdialogex.h"
#include "Mesg.h"

// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CString n, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_DIALOG, pParent)
	, m_mesg(_T(""))
{
	name = n;
	memcpy(mesg.to, n, n.GetLength());
	memcpy(mesg.from, theApp.username, theApp.username.GetLength());
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_mesg);
	DDV_MaxChars(pDX, m_mesg, 140);
	DDX_Control(pDX, IDC_LIST_HIS, m_his);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatDlg::OnBnClickedButtonSend)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CChatDlg 消息处理程序

void CChatDlg::OnBnClickedButtonSend() //发送
{
	UpdateData();
	int len = m_mesg.GetLength();

	if (len != 0) {
		mesg.length = len;
		memcpy(mesg.content, m_mesg, len*2);
		memcpy(mesg.from, theApp.username, theApp.username.GetLength()*2);
		memcpy(mesg.to, name, name.GetLength()*2);

		theApp.sock.SendTextData(mesg);

		this->show(mesg, theApp.username);

		memset(&mesg, 0, sizeof(mesg));
		m_mesg = "";
		UpdateData(false);
	}
}


void CChatDlg::show(Mesg mesg, CString who)
{
	CString s_time; //获取系统时间 　　
	CTime tm = CTime::GetCurrentTime();

	if (tm - lasttime >= 5) {
		s_time = tm.Format("%Y年%m月%d日 %X");
		m_his.InsertString(pos, s_time);
		pos++;
	}
	lasttime = tm;
	m_his.InsertString(pos, who +CString("：") + mesg.content);
	pos++;
}


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChatDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ChangeState();
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CChatDlg::ChangeState()
{
	SetWindowTextW(state ? name + CString(" [在线]") : name + CString(" [离线]"));
}
