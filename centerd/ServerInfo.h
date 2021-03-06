#ifndef __SERVER_INFO__
#define __SERVER_INFO__

#include <unordered_map>

#include "CPackage.h"
#include "CRpc.h"
#include "Common.h"
#include "CConfigParser.h"
#include "CBaseServer.h"
#include "CConnState.h"

namespace XEngine
{

//一个服务器信息
struct ServerInfo
{
public:
    ServerInfo();
    ~ServerInfo();
    void SetState(SERVER_STATE state);
public:
    SERVER_STATE m_State;
    SERVER_TYPE m_Type;
    SERVER_ID m_Id;
    int m_Port;
};

}

#endif //__SERVER_INFO__

