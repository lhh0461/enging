#include <stdint.h>

#include "Entity.h"
#include "EntityMgr.h"

namespace XEngine
{

CEntityMgr::CEntityMgr(ENTITY_ID init_id)
    :m_InitEntityId(init_id) ,m_NextEntityId(init_id)
{

}

CEntityMgr::~CEntityMgr()
{
    std::unordered_map<ENTITY_ID, CEntity *>::iterator it;
    for (it = m_AllEntitys.begin(); it != m_AllEntitys.end(); ++it) {
        delete it->second;
    }
    m_AllEntitys.clear();
}

ENTITY_ID CEntityMgr::GenEntityId()
{
    if (m_NextEntityId > UINT32_MAX) {
        m_NextEntityId = m_InitEntityId;
    }
    m_NextEntityId++;
    return m_NextEntityId;
}

int CEntityMgr::AddEntity(ENTITY_ID entity_id, CEntity *entity)
{
    m_AllEntitys.insert(std::pair<ENTITY_ID, CEntity*>(entity_id, entity));
    return 0;
}

int CEntityMgr::DelEntity(ENTITY_ID id)
{
    std::unordered_map<ENTITY_ID, CEntity *>::iterator it;
    it = m_AllEntitys.find(id);
    if (it != m_AllEntitys.end()) {
        delete it->second;
        m_AllEntitys.erase(it);
    }
    return 0;
}

CEntity *CEntityMgr::GetEntity(ENTITY_ID id)
{
    std::unordered_map<ENTITY_ID, CEntity *>::iterator it;
    it = m_AllEntitys.find(id);
    if (it != m_AllEntitys.end()) {
        return it->second;
    }
    return NULL;
}

int CEntityMgr::AddGlobalEntity(ENTITY_TYPE etype, CMailBox *mailbox)
{
    std::unordered_map<ENTITY_TYPE, CMailBox *>::iterator it;
    it = m_GlobalEntitys.find(etype);
    if (it != m_GlobalEntitys.end()) {
        delete it->second;
        it->second = mailbox;
        return 0;
    }
    return 1;
}

CMailBox *CEntityMgr::GetGlobalEntity(ENTITY_TYPE etype)
{
    //TODO goto redis
    std::unordered_map<ENTITY_TYPE, CMailBox *>::iterator it;
    it = m_GlobalEntitys.find(etype);
    if (it != m_GlobalEntitys.end()) {
        return it->second;
    }
    return NULL;
}

}

