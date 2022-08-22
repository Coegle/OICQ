#pragma once
#include <string>

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
		State				1 byte
		Length of username	1 byte
		Username			<var>

		*** control packet format ***
		Type				1 byte
*/

#include <afxsock.h>
#include "CLoginDlg.h"
#include "ClistDlg.h"
#include "resource.h"
class SocketC :
public CSocket
{

public:
	SocketC();

	virtual ~SocketC();

	void CloseSocket();

	//接收一切信息
	void OnReceive(int errcode);

	//发送文本信息
	void SendTextData(Mesg mesg);

	//发送控制信息
	void SendControlMessage(CString username, CString pass, int type);		

	const static unsigned int length = PACKET_SIZE * 2;

	CLoginDlg* logindlg;
	
	CListDlg* listdlg;
}; 

