#ifndef __MAIL_BOX__
#define __MAIL_BOX__

#include "Common.h"

namespace XEngine
{

class CEntity;

//寻址用
class CMailBox
{
public:
    CMailBox();
    CMailBox(SERVER_ID sid, ENTITY_TYPE etype, ENTITY_ID eid);
    ~CMailBox();
public:
    SERVER_ID GetServerId() { return m_ServerId; };
    ENTITY_TYPE GetEntityType() { return m_EntityType; };
    ENTITY_ID GetEntityId() {return m_EntityId; };
private:
    CMailBox(CMailBox& other);
    CMailBox(const CMailBox&);
    CMailBox & operator=(const CMailBox &);
    CMailBox & operator=(CMailBox& other);
private:
    SERVER_ID m_ServerId;
    ENTITY_TYPE m_EntityType;
    ENTITY_ID m_EntityId;
};

}

#endif //__MAIL_BOX__

