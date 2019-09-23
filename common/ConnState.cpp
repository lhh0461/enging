#include "ConnState.h"

namespace XEngine
{

CConnState::CConnState(const char *ip, int port, int flag):
   m_Ip(ip), m_Port(port), m_ConnFlag(flag)
{

    m_RecvPackList = new std::list<CPackage *>();
    m_SendPackList = new std::list<CPackage *>();
}

CConnState::~CConnState()
{
    if (m_RecvPackList) {
        delete m_RecvPackList;
    }
    if (m_SendPackList) {
        delete m_SendPackList;
    }
}

}
