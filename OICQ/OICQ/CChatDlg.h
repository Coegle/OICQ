#pragma once

#include "Mesg.h"
// CChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CString n, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString name;
	CString m_mesg;
	afx_msg void OnBnClickedButtonSend();
	Mesg mesg;
	int pos = 0;
	int state = 0;
	CTime lasttime = 0;
	CListBox m_his;
	void show(Mesg mesg , CString who);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ChangeState();
};
