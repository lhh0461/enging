#ifndef __PACKAGE__
#define __PACKAGE__

class CBuffer;

/* 
 * |数据包格式|
 * |固定4字节|变长2字节|变长2字节|...|
 * |包长度|序号|命令ID|命令数据|
 */

//代表一个数据包
class CPackage
{
public:
    enum {
        //ERROR CODE 
        DEFUALT_PACKAGE_SIZE = 10240,
        HEADER_SIZE = 4,
        RESERVE_SIZE = 2,
        CMD_SIZE = 2,
    };
public:
    CPackage(size_t size = DEFUALT_PACKAGE_SIZE);
    CPackage(const char *buf, size_t size); //for unpack
    ~CPackage();
public:
    bool UnPackCmd(uint16_t & cmd);
    bool UnPackString(std::string & val);
    bool UnPackInt(int64_t & val);
    bool UnPackFloat(double & val);
    bool UnPackBool(bool & val);
    bool UnPackBytes(std::string & val);
    bool UnPackEnd(std::string & str);
public:
    bool PackCmd(uint16_t cmd);
    bool PackString(const std::string & val);
    bool PackInt(int64_t & val);
    bool PackFloat(double & val);
    bool PackBool(bool & val);
    bool PackBytes(const std::string & val);
    bool PackEnd(std::string & val);
public:
    const CBuffer * GetBuffer() { return m_Buff; };
    uint16_t GetCmd() { return 1; };
    //uint16_t GetSeq() { return 1; };
    int GetErrCode() { return m_iErrorCode; };
private:
    bool UnpackCore(msgpack::object &obj);
private:
    CBuffer *m_Buff;
    size_t m_iUnpackOffset;
    int m_iErrorCode;
};

#endif //__PACKAGE__

