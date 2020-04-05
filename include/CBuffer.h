#ifndef __BUFFER__
#define __BUFFER__

#include <stddef.h>

namespace XEngine
{

class CBuffer
{
public:
    enum {
        BUFFER_INIT_SIZE = 10240,
    };
public:
    CBuffer(size_t initsz = BUFFER_INIT_SIZE);
    CBuffer(const char *buf, size_t len);
    CBuffer(CBuffer& other);
    ~CBuffer();
    CBuffer(const CBuffer&) = delete;
    CBuffer & operator=(const CBuffer &) = delete;
    CBuffer & operator=(CBuffer& other);
public:
    char* GetData() { return m_data; }
    const char* GetData() const { return m_data; }
    size_t GetDataSize() const { return m_size; }
    void SetDataSize(size_t size) { m_size = size; }
    size_t GetBufSize() const { return m_alloc; }
    void Reset() { m_size = 0; }
    int Expand(size_t len);
    int Expand();
private:
    char* m_data;
    size_t m_size;
    size_t m_alloc;
};

}

#endif //__BUFFER__

