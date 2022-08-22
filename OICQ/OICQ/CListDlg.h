#pragma once
#include "CColorListBox.h"
#include "CChatDlg.h"

// CListDlg 对话框

class CListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CColorListBox m_list;
	virtual void OnCancel();
};
