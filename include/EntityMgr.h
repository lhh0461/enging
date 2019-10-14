#ifndef __ENTITY_MGR__
#define __ENTITY_MGR__

#include <string>
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
    int AddEntity(ENTITY_ID entity_id, CEntity *entity);
    int DelEntity(ENTITY_ID id);
    CEntity *GetEntity(ENTITY_ID id);
public:
    int AddGlobalEntity(ENTITY_TYPE etype, CMailBox *mailbox);
    CMailBox *GetGlobalEntity(ENTITY_TYPE etype);
public:
    ENTITY_ID GenEntityId();
    ENTITY_TYPE GetEntityTypeByName(std::string name);
    std::string GetEntityNameByType(ENTITY_TYPE entity_type);
private:
    CEntityMgr(CEntityMgr& other);
    CEntityMgr(const CEntityMgr&);
    CEntityMgr & operator=(const CEntityMgr &);
    CEntityMgr & operator=(CEntityMgr& other);
private:
    ENTITY_ID m_InitEntityId;
    ENTITY_ID m_NextEntityId;
    std::unordered_map<ENTITY_ID, CEntity *> m_AllEntitys; //所有的Entity列表
    std::unordered_map<ENTITY_TYPE, CMailBox *> m_GlobalEntitys; //全局实体只储他的mailbox
    std::unordered_map<std::string, ENTITY_TYPE> m_EntityName2Type;
    std::unordered_map<ENTITY_TYPE, std::string> m_EntityType2Name;
};

}

#endif //__ENTITY_MGR__

