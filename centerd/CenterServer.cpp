#include <sys/epoll.h>
//#include <string>
#include <iostream>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CenterServer.h"
#include "Log.h"
#include "NetTool.h"
#include "Cmd.h"
#include "ConfigParser.h"
#include "Package.h"

using namespace std;

namespace XEngine
{

CCenterServer::CCenterServer():
    m_epoll_fd(0), m_listen_fd(0)
{
    m_Config = new CConfigParser();
    m_Rpc = new CRpc();
}

CCenterServer::~CCenterServer()
{

}


int CCenterServer::FromRpcCall(CPackage *package)
{
    uint16_t cmd_id;
    package->UnPackCmd(cmd_id); 
    
    switch(cmd_id) {
        case MSG_CMD_RPC:
            m_Rpc->Dispatch(package);
            break;
        default:
            break;
    }
    LOG_INFO("FromRpcCall end");
    return 0;
}

}

