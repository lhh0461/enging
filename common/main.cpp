#include <unistd.h>
#include <getopt.h>

#include "../centerd/CenterServer.h"
#include "BaseServer.h"
#include "Log.h"
#include "Common.h"

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
                break;
            case 'g':
                g_Server = new XEngine::CBaseServer(XEngine::SERVER_TYPE_GAMED);
                LOG_DEBUG("start gamed server");
                break;
            case 'd':
                g_Server = new XEngine::CBaseServer(XEngine::SERVER_TYPE_GAMED);
                LOG_DEBUG("start dbd server");
                break;
            case ':':
                break;
            case '?':
                break;
            default:
                break;
        }

    } while (next_option !=-1);

    g_Server->Init();
    g_Server->Run();

    return 0;
}
