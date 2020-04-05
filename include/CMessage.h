#ifndef __CMESSAGE__
#define __CMESSAGE__

/* 
 * |数据包格式|
 * |固定4字节|变长2字节|变长2字节|...|
 * |包长度|序号|命令ID|命令数据|
 */

namespace XEngine
{

class CMessage
{
public:
    CPackage(size_t size = PKG_DEFUALT_SIZE); //for pack
    CPackage(const char *buf, size_t size); //for unpack
    ~CPackage();
private:
    CBuffer *m_Buff;
    CConnState *m_ConnStat;
};

}

#endif //__CMESSAGE__

