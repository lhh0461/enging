#include <sys/types.h>
#include <sys/socket.h>

#include "Log.h"
#include "Common.h"
#include "Package.h"
#include "ConnState.h"

namespace XEngine
{

CConnState::CConnState(int fd, const char *ip, int port, int flag)
   :m_Fd(fd), m_Ip(ip), m_Port(port), m_ConnFlag(flag),
    m_IsConnected(0), m_ServerId(0), m_ServerType(0)
{
    m_SendPackList = new std::list<CPackage *>();
}

CConnState::~CConnState()
{
    if (m_SendPackList) {
        delete m_SendPackList;
    }
}

int CConnState::SendPackage()
{
    std::list<CPackage *> *pSendList = GetSendPackList();
    //LOG_INFO("start send package,fd=%d",GetFd());
    while (!pSendList->empty() && GetSendBufLeftSize() > 0) {
        LOG_INFO("send msg start!!");
        while (!pSendList->empty() && GetSendBufLeftSize() > 0) {
            CPackage *pPackage = pSendList->front();
            size_t iPackageLen = pPackage->GetPkgLen();
            size_t iAvailableLen = 0;
            //已经处理过一部份数据，则算包长度时减去它
            if (m_CurPackPos > 0) {
                iPackageLen -= m_CurPackPos;
            }
            //数据包比我的缓冲区剩余区域还大
            if (iPackageLen > GetSendBufLeftSize()) {
                iAvailableLen = GetSendBufLeftSize();
                memcpy(GetSendCurBuf(),pPackage->GetPkgBuf() + m_CurPackPos,iAvailableLen); 
                m_CurPackPos = iPackageLen - GetSendBufLeftSize();
            } else {
                pSendList->pop_front();
                iAvailableLen = iPackageLen;
                memcpy(GetSendCurBuf(),pPackage->GetPkgBuf() + m_CurPackPos,iAvailableLen); 
                m_CurPackPos = 0;
            }

            SetSendBufLen(GetSendBufPos() + iAvailableLen);
            //如果我的发送缓冲区已经没有数据了，跳出循环
            if (GetSendBufLeftSize() > 0) {
                //TODO
            } else {
                break;
            }
            LOG_INFO("iPackageLen=%d!!",iPackageLen);
        }
        //到这一步，要不发送缓冲区已满，要不所有的包已经发送
        assert(pSendList->empty() || GetSendBufLeftSize() == 0);
        //把发送缓冲区的内容全部发出
        ssize_t res = send(GetFd(),GetSendBuf(),GetSendBufPos(),0);
        LOG_INFO("res=%d,bufpos=%d!!",res, GetSendBufPos());
        if (res < 0) {
            //重来
            if (errno == EINTR) {
                continue;
            }
            //重来
            else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            } else {
                LOG_ERROR("recv msg error");
                break;
            }
        //只发送了一部分数据，那么把数据从后往前面移动
        } else if (res < GetSendBufPos()) {
            SetSendBufLen(GetSendBufPos() - res);
            memmove(GetSendBuf(), GetSendBuf() + res, GetSendBufPos());
            break;
        } else {
            //继续发送
            SetSendBufLen(0);
        }
    }
    if (GetSendBufPos() > 0) {
        if (send(GetFd(),GetSendBuf(),GetSendBufPos(),0) < 0) {

        }
    }
}

}
