#pragma once
#include <afxsock.h>

class COICQServerDlg ;
class SocketL : public CSocket
{
public:
    SocketL();
    virtual ~SocketL();
public:
    COICQServerDlg* pdlg;
    void Init(UINT port, COICQServerDlg *dlg);
    UINT m_uPort;

public:

    virtual void OnAccept(int nErrorCode);
};
