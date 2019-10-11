#ifndef __CENTER_SERVER__
#define __CENTER_SERVER__

#include <unordered_map>

#include "Package.h"
#include "Rpc.h"
#include "ConfigParser.h"
#include "ConnState.h"

namespace XEngine
{

class CCenterServer : public CBaseServer
{
public:
    CCenterServer();
    ~CCenterServer();
    void Init();
    void Run();
protected:
    int FromRpcCall(CPackage *package);
private:
    int OnServerRegister(CPackage *package);
private:
     CLoadBalance *m_LoadBalance;
     CLoadBalance *m_LoadBalance;
};

}

#endif //__CENTER_SERVER__

