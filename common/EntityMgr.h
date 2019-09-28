#ifndef __ENTITY_MGR__
#define __ENTITY_MGR__

namespace XEngine
{

class CEntity;

class CEntityMgr
{
public:
    typedef uint32_t ENTITY_ID;
public:
    CEntityMgr(ENTITY_ID init_id);
    ~CEntityMgr();
public:
    ENTITY_ID AddEntity(CEntity *entity);
    int DelEntity(ENTITY_ID id);
    CEntity *GetEntity(ENTITY_ID id);
private:
    CEntityMgr(CEntityMgr& other);
    CEntityMgr(const CEntityMgr&);
    CEntityMgr & operator=(const CEntityMgr &);
    CEntityMgr & operator=(CEntityMgr& other);
private:
    ENTITY_ID GenEntityId();
private:
    ENTITY_ID m_NextEntityId;
    std::unordered_map<ENTITY_ID, CEntity *> m_AllEntitys;
};

}

#endif //__ENTITY_MGR__

