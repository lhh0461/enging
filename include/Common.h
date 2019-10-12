#ifndef __COMMON__
#define __COMMON__

#include <stdint.h>

namespace XEngine
{

typedef uint16_t SERVER_ID;
typedef uint8_t  SERVER_TYPE;
typedef uint16_t CMD_ID;
typedef uint32_t ENTITY_ID;
typedef uint8_t  ENTITY_TYPE;

enum {
    SERVER_TYPE_NULL = 0,
    SERVER_TYPE_CENTERD = 1,
    SERVER_TYPE_GATED = 2,
    SERVER_TYPE_GAMED = 3,
    SERVER_TYPE_FIGHTD = 4,
    SERVER_TYPE_DBD = 5,
    SERVER_TYPE_HTTPD = 6,
};

//package handle error code
enum {
    ERR_SUCCESS = 0,
    ERR_UNKOWN_CMD = -1,
    ERR_UNPACK_FAIL = -2,
};

#define CENTERD_ID (1)

}

#endif //__COMMON__
