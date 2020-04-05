#ifndef __PACKAGE__
#define __PACKAGE__

#include <string>
#include <iostream>

#include "CBuffer.h"
#include "CLog.h"
#include "Common.h"

/* 
 * |数据包格式|
 * |固定4字节|变长2字节|变长2字节|...|
 * |包长度|序号|命令ID|命令数据|
 */

namespace XEngine
{

class CBuffer;
class CConnState;

//代表一个数据包
class CPackage
{
public:
    typedef uint32_t PKG_HEADER_TYPE;
public:
    enum {
        PKG_DEFUALT_SIZE = 10240,
    };
public:
    CPackage(size_t size = PKG_DEFUALT_SIZE); //for pack
    CPackage(const char *buf, size_t size); //for unpack
    ~CPackage();
public:
    int UnPackCmd(CMD_ID & cmd);
    int UnPackString(std::string & val);
    int UnPackInt8(int8_t & value);
    int UnPackInt16(int16_t & value);
    int UnPackInt32(int32_t & value);
    int UnPackInt64(int64_t & value);
    int UnPackUInt8(uint8_t & value);
    int UnPackUInt16(uint16_t & value);
    int UnPackUInt32(uint32_t & value);
    int UnPackUInt64(uint64_t & value);
    int UnPackFloat(float & val);
    int UnPackDouble(double & val);
    int UnPackBool(bool & val);
public:
    int PackCmd(CMD_ID cmd);
    int PackString(const char * ptr, size_t len);
    int PackUInt8(uint8_t val);
    int PackUInt16(uint16_t val);
    int PackUInt32(uint32_t val);
    int PackUInt64(uint64_t val);
    int PackInt8(int8_t val);
    int PackInt16(int16_t val);
    int PackInt32(int32_t val);
    int PackInt64(int64_t val);
    int PackFloat(float val);
    int PackDouble(double val);
    int PackBool(bool val);
public:
    const char* GetPkgBuf();
    CConnState* GetPkgConn() { return m_ConnStat; };
    size_t GetPkgLen();
    size_t GetPkgDataLen();
    int GetErrCode() { return m_iErrorCode; };
private:
    CBuffer *m_Buff;
    size_t m_iUnpackOffset;
    int m_iErrorCode;
    CConnState *m_ConnStat;
};

}

#endif //__PACKAGE__

