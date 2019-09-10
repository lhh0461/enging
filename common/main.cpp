#include "BaseServer.h"

int main()
{
    XEngine::CBaseServer *pServer = new XEngine::CBaseServer();
    pServer->Init();
    pServer->Run();
}
