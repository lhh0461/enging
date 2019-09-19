#ifndef __CONN_STATE__
#define __CONN_STATE__

#include <list>

#include "Package.h"

namespace XEngine
{

//此结构代表一个网络连接
class CConnState 
{
public:
    enum {
        SEND_BUF_SIZE = 10240,
        RECV_BUF_SIZE = 10240,
    };
    //连接标识
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
    //接收缓冲区
    char *GetRecvBuf() { return m_RecvBuf; };
    char *GetRecvPosBuf() { return m_RecvBuf + m_RecvBufPos; };
    int GetRecvBufPos() { return m_RecvBufPos; };
    int GetRecvBufLeftSize() { return RECV_BUF_SIZE - m_RecvBufPos; };
    int SetRecvBufLen(int len) { m_RecvBufPos = len; };
    //发送缓冲区
    char *GetSendBuf() { return m_SendBuf; };
    char *GetSendPosBuf() { return m_SendBuf + m_SendBufPos; };
    int GetSendBufPos() { return m_SendBufPos; };
    int GetSendBufLeftSize() { return SEND_BUF_SIZE - m_SendBufPos; };
    int SetSendBufLen(int len) { m_SendBufPos = len; };
public:
    std::list<CPackage *> & GetRecvPackList() { return m_RecvPackList; };
    std::list<CPackage *> & GetSendPackList() { return m_SendPackList; };
public:
    int OnConnect() {};
    int OnDisConnect() {};
    int OnReConnect() {};
private:
    int m_ConnFd;
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

