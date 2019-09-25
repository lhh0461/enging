#include <string>
#include <iostream>
#include <msgpack.hpp>

#include "Package.h"
#include "Buffer.h"
#include "Log.h"

namespace XEngine
{

//for pack
CPackage::CPackage(size_t size):
    m_Buff(NULL), m_iUnpackOffset(0), m_iErrorCode(0)
{
    m_Buff= new CBuffer(size);
    m_Buff->SetDataSize(sizeof(PKG_HEADER_TYPE));
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

#define UNPACK_OBJ(obj) \
    do { \
        if (m_iErrorCode != 0) { \
            return false; \
        } \
        msgpack::object_handle result; \
        try  \
        { \
            msgpack::unpack(result, m_Buff->GetData(), m_Buff->GetDataSize(), m_iUnpackOffset); \
        } \
        catch(msgpack::insufficient_bytes e) \
        { \
            LOG_ERROR("CPackage:%s;e=%s", __FUNCTION__ ,e.what()); \
            m_iErrorCode = 1; \
            return false; \
        } \
        catch(msgpack::parse_error e) \
        { \
            m_iErrorCode = 2; \
            LOG_ERROR("CPackage:%s;e=%s", __FUNCTION__ ,e.what()); \
            return false; \
        } \
        obj = result.get(); \
        std::cout << obj << std::endl; \
    } while(0);

bool CPackage::UnPackCmd(PKG_CMD_TYPE & cmd)
{
    return UnPackInt(cmd);
}

bool CPackage::UnPackInt(int8_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::NEGATIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.i64;
    return true;
}

bool CPackage::UnPackInt(int16_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::NEGATIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.i64;
    return true;
}

bool CPackage::UnPackInt(int32_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::NEGATIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.i64;
    return true;
}

bool CPackage::UnPackInt(int64_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
//    if (obj.type != msgpack::type::NEGATIVE_INTEGER)
//    {
//        m_iErrorCode = 3;
//        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
//        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", msgpack::type::POSITIVE_INTEGER);
//        return false;
//    }
    value = obj.via.i64;
    return true;
}

bool CPackage::UnPackInt(uint8_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::POSITIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.u64;
    return true;
}

bool CPackage::UnPackInt(uint16_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::POSITIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.u64;
    return true;
}

bool CPackage::UnPackInt(uint32_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::POSITIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.u64;
    return true;
}

bool CPackage::UnPackInt(uint64_t & value)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::POSITIVE_INTEGER)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackInt;desc=unpack is not int;type=%d", obj.type);
        return false;
    }
    value = obj.via.u64;
    return true;
}

bool CPackage::UnPackString(std::string & val)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::STR)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackString;desc=unpack is not string;type=%d", obj.type);
        return false;
    }

    val.assign(std::string(obj.via.str.ptr, obj.via.str.size));
    return true;
}

bool CPackage::UnPackBool(bool & val)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (msgpack::type::BOOLEAN != obj.type)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackBool;desc=unpack is not boolean;type=%d", obj.type);
        return false;
    }
    val = obj.via.boolean;
    return true;
}

bool CPackage::UnPackFloat(float & val)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (msgpack::type::FLOAT64 != obj.type)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackFloat;desc=unpack is not float;type=%d", obj.type);
        return false;
    }
    val = obj.via.f64;
    return true;
}

bool CPackage::UnPackFloat(double & val)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (msgpack::type::FLOAT64 != obj.type)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackFloat;desc=unpack is not float;type=%d", obj.type);
        return false;
    }
    val = obj.via.f64;
    return true;
}

bool CPackage::UnPackBytes(std::string & val)
{
    msgpack::object obj;
    UNPACK_OBJ(obj);
    if (obj.type != msgpack::type::BIN)
    {
        m_iErrorCode = 3;
        LOG_ERROR("CPackage::UnpackBytes;desc=unpack is not bin;type=%d", obj.type);
        return false;
    }
    val.assign(std::string(obj.via.bin.ptr, obj.via.bin.size));
    return true;
}

//-----------------unpack end--------------------

//-----------------pack begin--------------------
bool CPackage::PackCmd(PKG_CMD_TYPE cmd)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(cmd);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackString(const char * ptr, size_t len)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack_str((uint32_t)len);
    packer.pack_str_body(ptr, (uint32_t)len);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackString(const std::string & val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(uint8_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(uint16_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(uint32_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(uint64_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(int8_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(int16_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(int32_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackInt(int64_t val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackFloat(double val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackBool(bool val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackBytes(const char * ptr, size_t len)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack_bin((uint32_t)len);
    packer.pack_bin_body(ptr, (uint32_t)len);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

bool CPackage::PackBytes(const std::string & val)
{
    msgpack::packer<CBuffer> packer(m_Buff);
    packer.pack(val);
    *(PKG_HEADER_TYPE *)m_Buff->GetData() = m_Buff->GetDataSize();
    return true;
}

//-----------------pack end----------------------

const char * CPackage::GetPkgBuf()
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

