#ifndef __SERVICE__
#define __SERVICE__

namespace XEngine
{

class CService
{
public:
    CService() = default;
    CService(std::string strName);
    ~CService();
    CService(const CService&) = delete;
    CService & operator=(const CService &) = delete;
    CService & operator=(CService& other);
public:
private:
    int mServiceId;
    CSaveData *m_save_data;
};

}

#endif //__SERVICE__

