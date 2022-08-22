#if !defined(AFX_DSOCKET_H__B098630F_5F44_11D6_8897_000B2B0F84B6__INCLUDED_)
#define AFX_DSOCKET_H__B098630F_5F44_11D6_8897_000B2B0F84B6__INCLUDED_
#include <afxsock.h>
#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "User_Mesg.h"


class COICQServerDlg;
class SocketC : public CSocket	
{
	// Attributes
public:

	COICQServerDlg* pdlg;

	const static unsigned int length = PACKET_SIZE * 2;
	CString name;
	// Operations
public:

	SocketC(COICQServerDlg *dlg);

	virtual ~SocketC();

	void OnReceive(int errcode);

	void SendControlMessage(int type);

	void SendTextData(Mesg mesg);

	virtual void OnClose(int nErrorCode);

	void SendNotification(int type, CString name);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSOCKET_H__B098630F_5F44_11D6_8897_000B2B0F84B6__INCLUDED_)
