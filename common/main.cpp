#include <unistd.h>
#include <getopt.h>

#include "CLog.h"
#include "Common.h"
#include "CBaseServer.h"

#include "../centerd/CenterServer.h"
#include "../dbd/DBProxyServer.h"
#include "../gamed/GameServer.h"

using namespace XEngine;

int main(int argc, char *argv[])
{
    
    int next_option;

    const char* const short_options = "hcgd";
    const struct option long_options[] =
    {
        {"help",0,NULL,'h'},
        {"centerd",0,NULL,'c'},
        {"gamed",0,NULL,'g'},
        {"dbd",0,NULL,'d'},
        {NULL, 0, NULL, 0}
    };

    do
    {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);
        switch (next_option)
        {
            case 'h':
                return 0;
            case 'c':
                g_Server = new XEngine::CCenterServer();
                LOG_DEBUG("start center server");
                break;
            case 'g':
                g_Server = new XEngine::CGameServer();
                LOG_DEBUG("start gamed server");
                break;
            case 'd':
                //g_Server = new XEngine::CDBProxyServer();
                //LOG_DEBUG("start dbd server");
                break;
            case ':':
                break;
            case '?':
                break;
            default:
                break;
        }

    } while (next_option !=-1);

    if (g_Server) {
        g_Server->Init();
        g_Server->Run();
    }

    return 0;
}
