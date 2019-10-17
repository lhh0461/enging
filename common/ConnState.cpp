#include "Common.h"
#include "ConnState.h"

namespace XEngine
{

CConnState::CConnState(int fd, const char *ip, int port, int flag)
   :m_Fd(fd), m_Ip(ip), m_Port(port), m_ConnFlag(flag),
    m_IsConnected(0), m_ServerId(0), m_ServerType(0)
{
    m_SendPackList = new std::list<CPackage *>();
}

CConnState::~CConnState()
{
    if (m_SendPackList) {
        delete m_SendPackList;
    }
}

}
