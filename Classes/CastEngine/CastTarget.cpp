#include "CastTarget.h"

#include "CastWorldModel.h"

CastTarget::CastTarget(CastTargetType type)
{
	m_type = type;
	m_position = CCPointZero;
}


CastTarget::~CastTarget(void)
{
}

void CastTarget::addTargetEntity( ICastEntity* target )
{

	m_entityList.push_back(target);
}

void CastTarget::setTargetPosition( CCPoint target )
{
	m_position = target;
}
