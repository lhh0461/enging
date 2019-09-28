#ifndef __ENTITY__
#define __ENTITY__

#include <Python.h>

namespace XEngine
{

class CEntity
{
public:
    CEntity() = default;
    CEntity(std::string strName);
    ~CEntity();
    CEntity(const CEntity&) = delete;
    CEntity & operator=(const CEntity &) = delete;
    CEntity & operator=(CEntity& other);
public:
    int WriteToDb();
private:
    int m_EntityId;
    int m_EntityType;
    <int, PyDBObject *> m_DBData;
    PyDictObject *m_TempData;
};

}

#endif //__ENTITY__

