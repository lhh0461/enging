#ifndef __CONN_STATE__
#define __CONN_STATE__

#include <list>

#include "Buffer.h"
#include "Package.h"

namespace XEngine
{

//连接的类型
enum eConnType
{
    CLIENT_TYPE= 1,
    SERVER_TYPE= 2,
};

class CConnState 
{
public:
    CConnState();
    ~CConnState();
    CBuffer & GetRecvBuf() { return m_RecvBuf; };
    CBuffer & GetSendBuf() { return m_SendBuf; };
    std::list<CPackage *> & GetRecvPackList() { return m_RecvPackList; };
    std::list<CPackage *> & GetSendPackList() { return m_SendPackList; };
private:
    int m_ConnType;
    CBuffer m_SendBuf;
    CBuffer m_RecvBuf;
    std::list<CPackage *> m_RecvPackList;
    std::list<CPackage *> m_SendPackList;
};

}

#endif //__CONN_STATE__

