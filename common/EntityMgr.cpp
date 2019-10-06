#include <stdint.h>

#include "Entity.h"
#include "EntityMgr.h"

CEntityMgr::CEntityMgr(ENTITY_ID init_id)
    :m_InitEntityId(init_id) ,m_NextEntityId(init_id)
{

}

CEntityMgr::~CEntityMgr()
{
    unordered_map<ENTITY_ID, CEntity *>::iterator it;
    for (it = m_AllEntitys.begin(); it != m_AllEntitys.end(); ++it)
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

ENTITY_ID CEntityMgr::AddEntity(CEntity *entity)
{
    ENTITY_ID id = this->GenEntityId();
    m_AllEntitys.insert(pair<ENTITY_ID, CEntity*>(id, entity));
    return id;
}

int CEntityMgr::DelEntity(ENTITY_ID id)
{
    unordered_map<ENTITY_ID, CEntity *>::iterator it;
    it = m_AllEntitys.find(id);
    if (it != m_AllEntitys.end()) {
        delete it->second;
        m_AllEntitys.erase(it);
    }
    return 0;
}

CEntity *CEntityMgr::GetEntity(ENTITY_ID entity_id)
{
    unordered_map<ENTITY_ID, CEntity *>::iterator it;
    it = m_AllEntitys.find(id);
    if (it != m_AllEntitys.end()) {
        return it->second;
    }
    return NULL;
}

int AddMailBox(ENTITY_TYPE etype, CMailBox *mailbox)
{
    unordered_map<etype, CMailBox *>::iterator it;
    it = m_GlobalEntitys.find(etype);
    if (it != m_AllEntitys.end()) {
        delete it->second;
        it->second = mailbox;
        return 0;
    }
    return 1;
}

CMailBox *GetMailBox(ENTITY_TYPE etype)
{
    //TODO goto redis
    unordered_map<etype, CMailBox *>::iterator it;
    it = m_GlobalEntitys.find(etype);
    if (it != m_AllEntitys.end()) {
        return it->second;
    }
    return NULL;
}

