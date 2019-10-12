#ifndef __ENTITY__
#define __ENTITY__

#include <Python.h>
#include "Common.h"

namespace XEngine
{

class CEntity
{
public:
    CEntity(ENTITY_TYPE type,ENTITY_ID id);
    ~CEntity();
public:
    ENTITY_ID GetEntityId() { return m_EntityId; };
    ENTITY_TYPE GetEntityType() { return m_EntityType; };
private:
    CEntity(CEntity& other);
    CEntity(const CEntity&);
    CEntity & operator=(const CEntity &);
    CEntity & operator=(CEntity& other);
private:
    ENTITY_ID m_EntityId;
    ENTITY_TYPE m_EntityType;
    PyObject *m_SaveData;
};

}

#endif //__ENTITY__

