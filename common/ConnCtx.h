#ifndef __CONN_CTX__
#define __CONN_CTX__

//连接的类型
enum eConnType
{
    CLIENT_TYPE= 1,
    SERVER_TYPE= 2,
};

class CConnCtx 
{
public:
    CConnCtx();
    CConnCtx();
    ~CConnCtx();
    CBuffer & GetRecvBuf() { return m_RecvBuf; };
    CBuffer & GetSendBuf() { return m_SendBuf; };
    std::list<CPackage *> & GetRecvPackList() { return m_RecvPackList; };
    std::list<CPackage *> & GetSendPackList() { return m_SendPackList; };
private:
    int m_type;

    CBuffer m_SendBuf;
    CBuffer m_RecvBuf;

    std::list<CPackage *> m_RecvPackList;
    std::list<CPackage *> m_SendPackList;
};

#endif //__CONN_CTX__

