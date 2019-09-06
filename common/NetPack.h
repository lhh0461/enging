#ifndef __BUFFER__
#define __BUFFER__

#include <msgpack.hpp>

class CNetPack
{
public:
    CBuffer();
    ~CBuffer();
public:
    uint32_t GetPackLen();
    uint32_t GetPackLen();
    char * GetBuf() { return };
    size_t GetDataLen();
    GetDataLen();
private:
    char buf[10240];
};

#endif //__BUFFER__
