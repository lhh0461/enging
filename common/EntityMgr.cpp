#include "Entity.h"
#include "EntityMgr.h"


CEntityMgr::CEntityMgr(ENTITY_ID init_id)
    :m_NextEntityId(init_id)
{

}

CEntityMgr::~CEntityMgr()
{

}

ENTITY_ID CEntityMgr::GenEntityId()
{

}

ENTITY_ID CEntityMgr::AddEntity(CEntity *entity)
{
    
}

int CEntityMgr::DelEntity(ENTITY_ID id)
{

}

CEntity *CEntityMgr::GetEntity(ENTITY_ID entity_id)
{

}
