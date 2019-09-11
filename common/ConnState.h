#ifndef __CONN_STATE__
#define __CONN_STATE__

#include <list>

#include "Buffer.h"
#include "Package.h"

namespace XEngine
{

class CConnState 
{
public:
    enum {
        SEND_BUF_SIZE = 10240,
        RECV_BUF_SIZE = 10240,
    };
    enum eConnFlag
    {
        CONN_CLIENT_FLAG = (1<<1),
        CONN_SERVER_FLAG = (1<<2),
        CONN_HARBOR_FLAG = (1<<3),
    };
public:
    CConnState() = default;
    CConnState(const char *ip, int port, int flag);
    ~CConnState();

public:
    char *GetRecvBuf() { return m_RecvBuf; };
    int GetRecvBufPos() { return m_RecvBufPos; };
    int GetRecvBufLeftSize() { return RECV_BUF_SIZE - m_RecvBufPos; };

    char *GetSendBuf() { return m_SendBuf; };
    int GetSendBufPos() { return m_SendBufPos; };
    int GetSendBufLeftSize() { return SEND_BUF_SIZE - m_SendBufPos; };

    std::list<CPackage *> & GetRecvPackList() { return m_RecvPackList; };
    std::list<CPackage *> & GetSendPackList() { return m_SendPackList; };
    //void *operator new(size_t size);
private:
    int m_ConnFlag;
    std::string m_Ip;
    int m_Port;
private:
    char m_SendBuf[SEND_BUF_SIZE];
    int m_SendBufPos;
    char m_RecvBuf[RECV_BUF_SIZE];
    int m_RecvBufPos;
    CPackage *m_CurPackage;
private:
    std::list<CPackage *> m_RecvPackList;
    std::list<CPackage *> m_SendPackList;
};

}

#endif //__CONN_STATE__

