#ifndef __BASE_SERVER__
#define __BASE_SERVER__

#include <unordered_map>

enum
{
    MAX_EVENT = 9999,
};

enum eClientType
{
    CLIENT_TYPE= 1,
    HARBOR_TYPE= 2,
};

class CClientCtx 
{
public:
    CClientCtx();
    ~CClientCtx();
private:
    int m_vfd;
    int m_type;
    CBuffer m_SendBuf;
    CBuffer m_RecvBuf;
};

class CBaseServer
{
public:
    CBaseServer();
    ~CBaseServer();
    virtual ~CBaseServer();
    virtual void Init();
    virtual void Run();
    void AddFdToEpoll(int fd);
    void ReadFdPacket(int fd);
private:
    int m_epoll_fd; 
    int m_listen_fd; 
    int m_vfd; 
    int m_tick; 
    CRpc *m_Rpc;
    std::unordered_map<int, CClientCtx *> *m_ConnStat;
};

#endif //__BASE_SERVER__
