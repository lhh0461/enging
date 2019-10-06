#ifndef __ENTITY_MGR__
#define __ENTITY_MGR__

#include <unordered_map>

#include "MailBox.h"
#include "Common.h"

namespace XEngine
{

class CEntity;

class CEntityMgr
{
public:
    CEntityMgr(ENTITY_ID init_id);
    ~CEntityMgr();
public:
    ENTITY_ID AddEntity(CEntity *entity);
    int DelEntity(ENTITY_ID id);
    CEntity *GetEntity(ENTITY_ID id);
public:
    int AddMailBox(ENTITY_TYPE etype, CMailBox *mailbox);
    CMailBox *GetMailBox(ENTITY_TYPE etype);
private:
    CEntityMgr(CEntityMgr& other);
    CEntityMgr(const CEntityMgr&);
    CEntityMgr & operator=(const CEntityMgr &);
    CEntityMgr & operator=(CEntityMgr& other);
private:
    ENTITY_ID GenEntityId();
private:
    ENTITY_ID m_InitEntityId;
    ENTITY_ID m_NextEntityId;
    std::unordered_map<ENTITY_ID, CEntity *> m_AllEntitys;
    std::unordered_map<ENTITY_TYPE, CMailBox *> m_GlobalEntitys; //全局实体只存储他的mailbox
};

}

#endif //__ENTITY_MGR__

