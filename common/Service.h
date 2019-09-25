#ifndef __BUFFER__
#define __BUFFER__

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
    CSaveData *m_save_data;
};

}

#endif //__BUFFER__

