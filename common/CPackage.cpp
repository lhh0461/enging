#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "CPackage.h"
#include "CBuffer.h"
#include "CLog.h"

namespace XEngine
{

//for pack
CPackage::CPackage(size_t size):
    m_Buff(NULL), m_iUnpackOffset(0), m_iErrorCode(0)
{
    m_Buff= new CBuffer(size);
    PackInt16(0);
}

//for unpack
CPackage::CPackage(const char *buf, size_t size):
    m_Buff(NULL), m_iUnpackOffset(0), m_iErrorCode(0)
{
    m_Buff = new CBuffer(buf, size);
}

CPackage::~CPackage()
{
    if (m_Buff) {
        delete m_Buff;
    }
}

//-----------------unpack begin------------------

int CPackage::UnPackCmd(CMD_ID & cmd)
{
    return UnPackUInt16(cmd);
}

int CPackage::UnPackInt8(int8_t & value)
{
    if (m_iUnpackOffset + sizeof(int8_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(int8_t *)pData; 
    m_iUnpackOffset += sizeof(int8_t);
    return 0;
}

int CPackage::UnPackInt16(int16_t & value)
{
    if (m_iUnpackOffset + sizeof(int16_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(int16_t *)pData; 
    m_iUnpackOffset += sizeof(int16_t);
    return 0;
}

int CPackage::UnPackInt32(int32_t & value)
{
    if (m_iUnpackOffset + sizeof(int32_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(int32_t *)pData; 
    m_iUnpackOffset += sizeof(int32_t);
    return 0;
}

int CPackage::UnPackInt64(int64_t & value)
{
    if (m_iUnpackOffset + sizeof(int64_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(int64_t *)pData; 
    m_iUnpackOffset += sizeof(int64_t);
    return 0;
}

int CPackage::UnPackUInt8(uint8_t & value)
{
    if (m_iUnpackOffset + sizeof(uint8_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(uint8_t *)pData; 
    m_iUnpackOffset += sizeof(uint8_t);
    return 0;
}

int CPackage::UnPackUInt16(uint16_t & value)
{
    if (m_iUnpackOffset + sizeof(uint16_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(uint16_t *)pData; 
    m_iUnpackOffset += sizeof(uint16_t);
    return 0;
}

int CPackage::UnPackUInt32(uint32_t & value)
{
    if (m_iUnpackOffset + sizeof(uint32_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(uint32_t *)pData; 
    m_iUnpackOffset += sizeof(uint32_t);
    return 0;
}

int CPackage::UnPackUInt64(uint64_t & value)
{
    if (m_iUnpackOffset + sizeof(uint32_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    value = *(uint32_t *)pData; 
    m_iUnpackOffset += sizeof(uint32_t);
    return 0;
}

int CPackage::UnPackString(std::string & value)
{
    if (m_iUnpackOffset + sizeof(uint32_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    m_iUnpackOffset += sizeof(uint32_t);

    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    int iLen = *(uint32_t *)pData; 
    if (m_iUnpackOffset + iLen >= m_Buff->GetBufSize()) {
        return -1;
    }

    value.assign(std::string(pData + sizeof(uint32_t), iLen));
    m_iUnpackOffset += iLen;
    return 0;
}

int CPackage::UnPackBool(bool & value)
{
    if (m_iUnpackOffset + sizeof(uint8_t) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    uint8_t val = *(uint8_t *)pData; 
    if (val != 0) {
        value = true;
    } else {
        value = false;
    }
    m_iUnpackOffset += sizeof(uint8_t);
    return 0;
}

int CPackage::UnPackFloat(float & val)
{
    if (m_iUnpackOffset + sizeof(float) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    val = *(float *)pData; 
    m_iUnpackOffset += sizeof(float);
    return 0;
}

int CPackage::UnPackDouble(double & val)
{
    if (m_iUnpackOffset + sizeof(double) >= m_Buff->GetBufSize()) {
        return -1;
    }
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    val = *(double *)pData; 
    m_iUnpackOffset += sizeof(double);
    return 0;
}

//-----------------unpack end--------------------

//-----------------pack begin--------------------

int CPackage::PackCmd(CMD_ID cmd)
{
    return PackUInt16(cmd);
}

int CPackage::PackUInt8(uint8_t val)
{
    if (m_iUnpackOffset + sizeof(uint8_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(uint8_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint8_t);
    return 0;
}

int CPackage::PackUInt16(uint16_t val)
{
    if (m_iUnpackOffset + sizeof(uint16_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(uint16_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint16_t);
    return 0;
}

int CPackage::PackUInt32(uint32_t val)
{
    if (m_iUnpackOffset + sizeof(uint32_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(uint32_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint32_t);
    return 0;
}

int CPackage::PackUInt64(uint64_t val)
{
    if (m_iUnpackOffset + sizeof(uint64_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(uint64_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint64_t);
    return 0;
}

int CPackage::PackInt8(int8_t val)
{
    if (m_iUnpackOffset + sizeof(int8_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(int8_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint8_t);
    return 0;
}

int CPackage::PackInt16(int16_t val)
{
    if (m_iUnpackOffset + sizeof(int16_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(int16_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint16_t);
    return 0;
}

int CPackage::PackInt32(int32_t val)
{
    if (m_iUnpackOffset + sizeof(int32_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(int32_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint32_t);
    return 0;
}

int CPackage::PackInt64(int64_t val)
{
    if (m_iUnpackOffset + sizeof(int64_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(int64_t *)pData = val; 
    m_iUnpackOffset += sizeof(uint64_t);
    return 0;
}

int CPackage::PackFloat(float val)
{
    if (m_iUnpackOffset + sizeof(float) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(float *)pData = val; 
    m_iUnpackOffset += sizeof(float);
    return 0;
}

int CPackage::PackDouble(double val)
{
    if (m_iUnpackOffset + sizeof(double) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(double *)pData = val; 
    m_iUnpackOffset += sizeof(double);
    return 0;
}

int CPackage::PackBool(bool val)
{
    if (m_iUnpackOffset + sizeof(uint8_t) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;
    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(uint8_t *)pData = val == true ? 1 : 0; 
    m_iUnpackOffset += sizeof(uint8_t);
    return 0;
}

int CPackage::PackString(const char* ptr, size_t len)
{
    if (m_iUnpackOffset + sizeof(len) >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand() != 0) return -1;

    char *pData = m_Buff->GetData() + m_iUnpackOffset;
    *(uint64_t *)pData = len; 
    m_iUnpackOffset += sizeof(uint64_t);

    if (m_iUnpackOffset + len >= m_Buff->GetBufSize()) 
        if (m_Buff->Expand(len) != 0) return -1;
   
    memcpy(pData + sizeof(uint64_t), ptr, len);
    return 0;
}

//-----------------pack end----------------------

const char* CPackage::GetPkgBuf()
{ 
    return m_Buff->GetData();
};

size_t CPackage::GetPkgLen()
{
    return m_Buff->GetDataSize();
};

size_t CPackage::GetPkgDataLen()
{
    return m_Buff->GetDataSize() - sizeof(PKG_HEADER_TYPE);
};

}

