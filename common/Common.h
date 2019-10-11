#ifndef __COMMON__
#define __COMMON__

namespace XEngine
{

typedef uint8_t SERVER_TYPE;
typedef uint16_t SERVER_ID;
typedef uint16_t ENTITY_TYPE;
typedef uint32_t ENTITY_ID;

enum {
    SERVER_TYPE_NULL = 0,
    SERVER_TYPE_CENTERD = 1,
    SERVER_TYPE_GATED = 2,
    SERVER_TYPE_GAMED = 3,
    SERVER_TYPE_FIGHTD = 4,
    SERVER_TYPE_DBD = 5,
    SERVER_TYPE_HTTPD = 6,
};

#define CENTERD_ID (1)

}

#endif //__COMMON__

