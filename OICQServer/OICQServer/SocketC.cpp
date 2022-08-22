
/*		***  Text packet format ***
		Type				1 byte
		Message Class		<var>

		*** Login/Regist packet format ***
		Type				1 byte
		Length of Username	1 byte
		Length of Password	1 byte
		Username			<var>
		Password			<var>

		*** state packet format ***
		Type				1 byte
		Length of username	1 byte
		Username			<var>

		*** control packet format ***
		Type				1 byte
*/


#include "pch.h"

#include "SocketC.h"
#include "OICQServer.h"
#include "OICQServerDlg.h"
#include <fstream>
#include "User_Mesg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



SocketC::SocketC(COICQServerDlg *dlg)
{
	pdlg = dlg;
}

SocketC::~SocketC()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(SocketC, CSocket)
	//{{AFX_MSG_MAP(SocketC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////



//任何数据到达时都调用此函数	
void SocketC::OnReceive(int errcode)
{
	wchar_t data[PACKET_SIZE] = { 0 };
	wchar_t name[USER_SIZE] = { 0 };
	wchar_t pass[PASS_SIZE] = { 0 };
	unsigned int retvalue;
	User tmp;
	int len1, len2;
	std::map<CString, User>::iterator it_registed_user;
	std::map<CString, SocketC*>::iterator it_msgsock;
	std::multimap<CString, Mesg>::iterator it_unsend_msg;
	Mesg mesg;

	retvalue = this->Receive(data, length);
	if (retvalue == SOCKET_ERROR)return;

	switch (data[0]) {
	case TYPE_LOGIN://						登录
		len1 = data[1];
		len2 = data[2];

		memcpy(name, &data[3], len1 * 2);
		memcpy(pass, &data[3 + len1], len2 * 2);

		tmp.password = pass;
		tmp.username = name;

		it_registed_user = theApp.Registed_User.find(tmp.username);
		if (it_registed_user != theApp.Registed_User.end() && it_registed_user->second.password == tmp.password) { //登录成功

			//向客户端发送登录成功信息
			this->SendControlMessage(TYPE_SUCCEED);

			//在界面中更新用户在线信息
			pdlg->list_online.InsertString(theApp.online, tmp.username);
			theApp.online++;

			//在后台更新用户在线信息
			theApp.Registed_User[tmp.username].alive = true;
			
			it_msgsock = theApp.msgsock.find(name);
			if (it_msgsock != theApp.msgsock.end()) {
				it_msgsock->second->SendControlMessage(TYPE_EXIT);
				theApp.msgsock.erase(it_msgsock);
			}
			//保存在通信套接字映射列表中
			theApp.msgsock[tmp.username] = this;
			this->name = tmp.username;

			//向其发送缓存消息
			it_unsend_msg = theApp.Unsend_Mesg.find(tmp.username);
			while (it_unsend_msg != theApp.Unsend_Mesg.end())
			{
				this->SendTextData(it_unsend_msg->second);
				theApp.Unsend_Mesg.erase(it_unsend_msg);
				it_unsend_msg = theApp.Unsend_Mesg.find(tmp.username);
			}

			//在界面中删除缓存消息
			int cnt = pdlg->list_msg.GetItemCount();
			for (int i = 0; i < cnt; i++) {
				if (pdlg->list_msg.GetItemText(i, 0) == tmp.username) {
					pdlg->list_msg.DeleteItem(i);
					i--;
				}
			}

			//在界面中添加上线事件
			CString str_eve = CString("上线：") + name;
			pdlg->list_eve.InsertString(0, str_eve);
			
			//向其余在线用户发送上线通知
			for (it_msgsock = theApp.msgsock.begin(); it_msgsock != theApp.msgsock.end(); it_msgsock++) {
				if(it_msgsock->first != name) it_msgsock->second->SendNotification(TYPE_ONLINE, name);
			}

			//向该用户发送其余用户状态
			for (auto it_msgsock = theApp.Registed_User.begin(); it_msgsock != theApp.Registed_User.end(); it_msgsock++) {
				if (it_msgsock->first != name) {
					this->SendNotification((it_msgsock->second.alive ? TYPE_ONLINE : TYPE_OFFLINE), it_msgsock->first);
				}
			}
		}
		else {
			this->SendControlMessage(TYPE_FAILED);

			CString str_eve = CString("登录失败：") + name;
			pdlg->list_eve.InsertString(0, str_eve);
		}
		break;

	case TYPE_REGIST://						注册
		len1 = data[1];
		len2 = data[2];

		memcpy(name, &data[3], len1 * 2);
		memcpy(pass, &data[3 + len1], len2 * 2);

		tmp.username = name;
		tmp.password = pass;

		it_registed_user = theApp.Registed_User.find(tmp.username);
		if (it_registed_user == theApp.Registed_User.end()) {		//注册成功

			//向客户端发送注册成功信息
			this->SendControlMessage(TYPE_SUCCEED);

			//保存在通信套接字映射列表中
			theApp.msgsock[tmp.username] = this;
			this->name = tmp.username;

			//在界面中更新用户在线信息
			pdlg->list_online.InsertString(theApp.online, tmp.username);
			theApp.online++;

			//在后台更新用户在线信息
			theApp.Registed_User[tmp.username] = tmp;
			theApp.Registed_User[tmp.username].alive = true;

			//在界面中添加注册事件
			CString str_eve = CString("注册成功：") + tmp.username;
			pdlg->list_eve.InsertString(0, str_eve);
			theApp.msgsock[tmp.username] = this;
			this->name = tmp.username;

			//将用户信息写入数据文件
			std::ofstream writer(theApp.PATH, std::ios::binary | std::ios::app);
			writer.write((char*)&name, USER_SIZE * 2);
			writer.write((char*)&pass, PASS_SIZE * 2);
			writer.close();

			//向其余在线用户发送上线通知
			for (it_msgsock = theApp.msgsock.begin(); it_msgsock != theApp.msgsock.end(); it_msgsock++) {
				if (it_msgsock->first != name) it_msgsock->second->SendNotification(TYPE_ONLINE, name);
			}

			//向该用户发送其余用户状态
			for (it_registed_user = theApp.Registed_User.begin(); it_registed_user != theApp.Registed_User.end(); it_registed_user++) {
				if (it_registed_user->first != name) {
					this->SendNotification((it_registed_user->second.alive ? TYPE_ONLINE : TYPE_OFFLINE), it_registed_user->first);
				}
			}

		}
		else {
			CString str_eve = CString("注册失败：") + tmp.username;
			pdlg->list_eve.InsertString(0, str_eve);

			this->SendControlMessage(TYPE_FAILED);
		}

		break;

	case TYPE_TEXT://						信息
		memcpy(&mesg, &data[1], sizeof(mesg));

		CString str_eve = CString("发送消息：") + mesg.from + CString(" --> ") + mesg.to;
		pdlg->list_eve.InsertString(0, str_eve);

		if (theApp.msgsock.find(mesg.to) == theApp.msgsock.end()) {		//不在线

			theApp.Unsend_Mesg.insert(std::make_pair(mesg.to, mesg));

			int nRow = pdlg->list_msg.InsertItem(0, mesg.to);
			pdlg->list_msg.SetItemText(nRow, 1, mesg.from);
			pdlg->list_msg.SetItemText(nRow, 2, mesg.content); 
		}
		else {															//在线
			theApp.msgsock[mesg.to]->SendTextData(mesg);
		}
		break;

	}
}

//		发送控制报文数据：登录/注册成功/失败
void SocketC::SendControlMessage(int type)
{
	wchar_t packet[PACKET_SIZE] = { 0 };
	packet[0] = type;

	Send(packet, sizeof(packet));

	if (type == TYPE_FAILED) {
		CSocket::Close();
	}

}

//		发送文本数据
void SocketC::SendTextData(Mesg mesg)
{
	wchar_t packet[PACKET_SIZE] = { 0 };

	packet[0] = TYPE_TEXT;
	memcpy(&packet[1], &mesg, sizeof(mesg)); 

	Send(packet, sizeof(packet));
}

//		发送通知：上线/下线
void SocketC::SendNotification(int type, CString name)
{
	wchar_t packet[PACKET_SIZE] = { 0 };
	int len = name.GetLength();

	packet[0] = type;
	packet[1] = len;
	memcpy(&packet[2], name, len * 2);

	Send(packet, sizeof(packet));
}

//		客户端退出
void SocketC::OnClose(int nErrorCode)
{
	CString str_eve = CString("下线：") + name;
	pdlg->list_eve.InsertString(0, str_eve);

	//delete theApp.msgsock[name];
	theApp.msgsock.erase(this->name);

	pdlg->list_online.DeleteString(pdlg->list_online.FindString(0, this->name));

	std::map<CString, SocketC*>::iterator it;
	theApp.Registed_User[name].alive = false;

	for (it = theApp.msgsock.begin(); it != theApp.msgsock.end(); it++) {
		it->second->SendNotification(TYPE_OFFLINE, this->name);
	}

	theApp.online--;
	
	CSocket::OnClose(nErrorCode);
}


