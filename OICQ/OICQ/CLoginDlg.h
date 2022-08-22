
// OICQDlg.h: 头文件
//

#pragma once


// COICQDlg 对话框
class CLoginDlg : public CDialogEx
{
// 构造
public:
	CLoginDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OICQ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedLogin();

	afx_msg void OnBnClickedRegist();

	virtual void OnOK();
	CButton btn_login;
	CButton btn_reg;
	CString edit_name;
	CString edit_pass;
	CString m_caption;
	CString edit_ipaddr;
	
};
