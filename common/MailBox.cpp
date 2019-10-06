#include "MailBox.h"

namespace XEngine
{


CMailBox::CMailBox()
{
}

CMailBox::CMailBox(SERVER_ID sid, ENTITY_TPYE etype, ENTITY_ID eid)
    :m_ServerId(sid), m_EntityType(etype), m_EntityId(eid)
{
}

CMailBox::~CMailBox()
{
}

}
