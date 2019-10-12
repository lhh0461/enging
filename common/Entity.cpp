#include <stdint.h>

#include "Entity.h"

namespace XEngine
{

CEntity::CEntity(ENTITY_TYPE type, ENTITY_ID id)
    :m_EntityId(id) ,m_EntityType(type)
{
    m_SaveData = PyDict_New();
}

CEntity::~CEntity()
{
    Py_DECREF(m_SaveData);
}

}
