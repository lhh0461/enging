#include "BaseServer.h"
#include "Common.h"

int main(int argc, char *argv[])
{
    XEngine::CBaseServer *pServer = new XEngine::CBaseServer(XEngine::SERVER_TYPE_GAMED);
    pServer->Init();
    pServer->Run();
}
