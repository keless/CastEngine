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

const std::vector<ICastEntity*>& CastTarget::getEntityList() 
{ 
	return m_entityList; 
}

void CastTarget::clearTargetEntities()
{
	m_entityList.clear();
}

void CastTarget::addTargetEntity( ICastEntity* target )
{
	m_type = CTT_ENTITIES;
	m_entityList.push_back(target);
}

void CastTarget::setTargetPosition( CCPoint target )
{
	m_position = target;
}

void CastTarget::validateTargets()
{
	CastWorldModel* world = CastWorldModel::get();
	for( int i = m_entityList.size()-1; i>= 0; i--)
	{
		if( !world->isValid( m_entityList[i] ) )
		{
			m_entityList.erase( m_entityList.begin() + i );
		}
	}
}

bool CastTarget::hasValidTarget()
{
	if( m_type == CTT_ENTITIES ) 
	{
		validateTargets();
		return m_entityList.size() > 0;
	}else {
		return true;
	}
	


}