#ifndef __PACKAGE__
#define __PACKAGE__

#include <string>
#include <iostream>
#include <msgpack.hpp>

#include "Buffer.h"
#include "Log.h"
#include "Commmon.h"

/* 
 * |数据包格式|
 * |固定4字节|变长2字节|变长2字节|...|
 * |包长度|序号|命令ID|命令数据|
 */

namespace XEngine
{

class CBuffer;
class CMailBox;

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
    bool UnPackCmd(CMD_ID & cmd);
    bool UnPackString(std::string & val);
    bool UnPackInt(int8_t & value);
    bool UnPackInt(int16_t & value);
    bool UnPackInt(int32_t & value);
    bool UnPackInt(int64_t & value);
    bool UnPackInt(uint8_t & value);
    bool UnPackInt(uint16_t & value);
    bool UnPackInt(uint32_t & value);
    bool UnPackInt(uint64_t & value);
    bool UnPackFloat(float & val);
    bool UnPackFloat(double & val);
    bool UnPackBool(bool & val);
    bool UnPackBytes(std::string & val);
public:
    bool PackCmd(CMD_ID cmd);
    bool PackString(const char * ptr, size_t len);
    bool PackString(const std::string & val);
    bool PackInt(uint8_t val);
    bool PackInt(uint16_t val);
    bool PackInt(uint32_t val);
    bool PackInt(uint64_t val);
    bool PackInt(int8_t val);
    bool PackInt(int16_t val);
    bool PackInt(int32_t val);
    bool PackInt(int64_t val);
    bool PackFloat(float val);
    bool PackFloat(double val);
    bool PackBool(bool val);
    bool PackBytes(const char * ptr, size_t len);
    bool PackBytes(const std::string & val);
    bool PackMailBox(const CMailBox *mailbox);
public:
    const char * GetPkgBuf();
    size_t GetPkgLen();
    size_t GetPkgDataLen();
    int GetErrCode() { return m_iErrorCode; };
    void ResetUnpackOffset() { m_iUnpackOffset = 0; };
private:
    CBuffer *m_Buff;
    size_t m_iUnpackOffset;
    int m_iErrorCode;
};

}

#endif //__PACKAGE__

