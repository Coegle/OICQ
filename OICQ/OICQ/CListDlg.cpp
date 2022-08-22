// CListDlg.cpp: 实现文件
//

#include "pch.h"
#include "OICQ.h"
#include "CListDlg.h"
#include "afxdialogex.h"


// CListDlg 对话框

IMPLEMENT_DYNAMIC(CListDlg, CDialogEx)

CListDlg::CListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIST_DIALOG, pParent)
{
}

CListDlg::~CListDlg()
{
	//while (theApp.dlgs.size() != 0) {
	//	auto it = theApp.dlgs.begin();
	//	delete it->second;
	//	theApp.dlgs.erase(it);
	//}
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CListDlg, CDialogEx)
END_MESSAGE_MAP()

// CListDlg 消息处理程序

BOOL CListDlg::OnInitDialog()
{
	GetDlgItem(IDC_STATIC)->SetWindowTextW(theApp.username);
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CListDlg::OnCancel()
{
	while (theApp.dlgs.size() != 0) {
		auto it = theApp.dlgs.begin();
		delete it->second;
		theApp.dlgs.erase(it);
	}
	CDialogEx::OnCancel();
}
