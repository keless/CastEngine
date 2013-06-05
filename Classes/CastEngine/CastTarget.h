
#ifndef _CASTTARGET_H_
#define _CASTTARGET_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "CastEntity.h"

/*
 CastTarget 

  used to describe what a cast ability is trying to affect

*/

//TODO: LIKELY CRASH BUG:  m_entityList will hold pointers to invalid entities when they get killed

enum CastTargetType
{
	CTT_ENTITIES,			//one or more entities
	CTT_WORLD_POSITION,
	CTT_RELATIVE_POSITION,
};

class CastTarget : public CCObject
{
	CastTargetType m_type;

	CCPoint m_position;
	std::vector<ICastEntity*> m_entityList;

public:
	CastTarget(CastTargetType type = CTT_ENTITIES);
	~CastTarget(void);

	void addTargetEntity( ICastEntity* target );
	void setTargetPosition( CCPoint target );
};

#endif

