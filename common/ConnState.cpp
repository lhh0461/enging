#include "ConnState.h"

namespace XEngine
{

CConnState::CConnState(const char *ip, int port, int flag):
   m_Ip(ip), m_Port(port), m_ConnFlag(flag)
{

}

CConnState::~CConnState()
{

}

}
