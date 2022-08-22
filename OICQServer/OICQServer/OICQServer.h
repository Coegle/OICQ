
// OICQServer.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include "SocketC.h"
#include "SocketL.h"
#include <map>
#include "User_Mesg.h"

// COICQServerApp:
// 有关此类的实现，请参阅 OICQServer.cpp
//

class COICQServerApp : public CWinApp
{
public:
	COICQServerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现
	std::map<CString,SocketC*> msgsock;						//通信套接字
	SocketL sockL;											//监听套接字
	const UINT Port = 6000;									//端口
	const char* PATH = "user_info.dat";						//用户信息文件路径
	int online = 0;											//在线人数
	std::map<CString, User> Registed_User;					//存储已经注册的用户信息
	std::multimap<CString, Mesg>Unsend_Mesg;				//未发送消息缓存
	DECLARE_MESSAGE_MAP()
};

extern COICQServerApp theApp;
