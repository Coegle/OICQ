
// OICQ.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include "SocketC.h"
#include "map"
// COICQApp:
// 有关此类的实现，请参阅 OICQ.cpp
//

class COICQApp : public CWinApp
{
public:
	COICQApp();

// 重写
public:
	virtual BOOL InitInstance();
// 实现
		
	SocketC sock;							//通信套接字

	CString username;						//登录用户名

	std::map<CString, CChatDlg*> dlgs;		//对话框指针映射

	CString ipaddress = _T("127.0.0.1");	//IP地址

	const UINT Port = 6000;					//端口

	DECLARE_MESSAGE_MAP()
};

extern COICQApp theApp;
