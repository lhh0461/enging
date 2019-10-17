#ifndef __CONN_STATE__
#define __CONN_STATE__

#include <list>
#include <string>
#include "Common.h"

namespace XEngine
{

class CPackage;

//连接标识
enum eConnFlag
{
    CONN_MAILBOX_FLAG = (1<<1), //是主动连出去的
    CONN_ACCEPTED_FLAG = (1<<2),//是被动连接的
    CONN_CLIENT_FLAG = (1<<3), //是客户端的连接
    CONN_HARBOR_FLAG = (1<<4), //是服务器之间的连接
    CONN_CREATING_FLAG = (1<<5), //已经认证
};

//连接状态机所处的状态
enum eConnStat
{
    CONN_VERIFY_STAT = 1, //客户端认证服务器
    CONN_SIGN_STAT = 2, //服务器认证客户端
    CONN_VERSION_STAT = 3, //版本认证阶段
    CONN_RPC_STAT = 4, //已经进入RPC通讯阶段
};

//此结构代表一个网络连接
class CConnState 
{
public:
    enum {
        SEND_BUF_SIZE = 10240,
        RECV_BUF_SIZE = 10240,
    };
public:
    CConnState() = default;
    CConnState(int fd, const char *ip, int port, int flag);
    ~CConnState();
public:
    //连接标识
    int InFlag(int flag) { return m_ConnFlag & flag; };
    int SetFlag(int flag) { return m_ConnFlag |= flag; };
    int UnSetFlag(int flag) { return m_ConnFlag &= ~flag; };
public:
    //
    int SetConnected(int flag) { m_IsConnected = flag; };
    int IsConnected() { return m_IsConnected; };
public:
    //接收缓冲区开始位置
    char *GetRecvBuf() { return m_RecvBuf; };
    //缓冲区下次写入字节
    char *GetRecvCurBuf() { return m_RecvBuf + m_RecvBufPos; };
    int GetRecvBufPos() { return m_RecvBufPos; };
    int GetRecvBufLeftSize() { return RECV_BUF_SIZE - m_RecvBufPos; };
    void SetRecvBufLen(int len) { m_RecvBufPos = len; };
    //发送缓冲区
    char *GetSendBuf() { return m_SendBuf; };
    char *GetSendCurBuf() { return m_SendBuf + m_SendBufPos; };
    int GetSendBufPos() { return m_SendBufPos; };
    int GetSendBufLeftSize() { return SEND_BUF_SIZE - m_SendBufPos; };
    int SetSendBufLen(int len) { m_SendBufPos = len; };
    //获得连接信息
    int GetFd() { return m_Fd; };
    int GetServerId() { return m_ServerId; };
    int SetServerId(SERVER_ID id) { m_ServerId = id; };
    int GetServerType() { return m_ServerType; };
    int SetServerType(SERVER_TYPE type) { m_ServerType = type; };
public:
    int PushSendPackList(CPackage *package) { m_SendPackList->push_back(package); };
    std::list<CPackage *> *GetSendPackList() { return m_SendPackList; };
private:
    int m_Fd;
    SERVER_TYPE m_ServerId; //是服务器的连接才有用
    SERVER_TYPE m_ServerType; //是服务器的连接才有用
    int m_ConnFlag;
    int m_ConnStat;
    int m_IsConnected;
    std::string m_Ip;
    int m_Port;
private:
    char m_SendBuf[SEND_BUF_SIZE];
    int m_SendBufPos;
    char m_RecvBuf[RECV_BUF_SIZE];
    int m_RecvBufPos;
    CPackage *m_CurPackage;
private:
    std::list<CPackage *> *m_SendPackList;
};

}

#endif //__CONN_STATE__

