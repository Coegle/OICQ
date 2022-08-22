#include "pch.h"
#include "SocketL.h"
#include "OICQServerDlg.h"
#include "OICQServer.h"
#include "SocketC.h"

SocketL::SocketL(){}
SocketL::~SocketL(){}
void SocketL::Init(UINT port, COICQServerDlg* dlg)
{
	pdlg = dlg;
	m_uPort = port;

	if (false == this->Create(m_uPort)) {
		AfxMessageBox(_T("´´½¨Ì×½Ó×ÖÊ§°Ü"));
		return;
	}

	this->Listen();  
}

void SocketL::OnAccept(int nErrorCode)
{
	SocketC* client = new SocketC(pdlg);
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	int addrLen = sizeof(addr);

	Accept(*client, (SOCKADDR*)&addr, &addrLen);

	CSocket::OnAccept(nErrorCode);
}


