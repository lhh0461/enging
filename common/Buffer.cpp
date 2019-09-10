#include <stdlib.h>
#include <cstring>
#include <new>
#include "Buffer.h"

namespace XEngine
{

CBuffer::CBuffer(size_t initsz):
    m_size(0), m_alloc(initsz), m_data(NULL)
{
    if (initsz > 0) {
        m_data = (char*)::malloc(initsz);
        if(!m_data) {
            throw std::bad_alloc();
        }
    }
}

CBuffer::CBuffer(const char *buf, size_t len):
    m_size(len), m_alloc(len), m_data(NULL)
{
    if (buf != NULL && len > 0) {
        m_data = (char*)::malloc(len);
        if(!m_data) {
            throw std::bad_alloc();
        }
        std::memcpy(m_data, buf, len);
    }
}

CBuffer::CBuffer(CBuffer& other) :
    m_size(other.m_size), m_data(other.m_data), m_alloc(other.m_alloc)
{
    other.m_size = other.m_alloc = 0;
    other.m_data = NULL;
}

CBuffer::~CBuffer()
{
    ::free(m_data);
}

CBuffer& CBuffer::operator=(CBuffer& other)
{
    ::free(m_data);

    m_size = other.m_size;
    m_alloc = other.m_alloc;
    m_data = other.m_data;

    other.m_size = other.m_alloc = 0;
    other.m_data = NULL;

    return *this;
}

void CBuffer::write(const char* buf, size_t len)
{
    if(m_alloc - m_size < len) {
        Expand(len);
    }
    std::memcpy(m_data + m_size, buf, len);
    m_size += len;
}

void CBuffer::Expand(size_t len)
{
    size_t nsize = (m_alloc > 0) ?
        m_alloc * 2 : BUFFER_INIT_SIZE;

    while (nsize < m_size + len) {
        size_t tmp_nsize = nsize * 2;
        if (tmp_nsize <= nsize) {
            nsize = m_size + len;
            break;
        }
        nsize = tmp_nsize;
    }

    void* tmp = ::realloc(m_data, nsize);
    if (!tmp) {
        throw std::bad_alloc();
    }

    m_data = static_cast<char*>(tmp);
    m_alloc = nsize;
}

}   // namespace XEngine

