#include "GameEntity.h"


GameEntity::GameEntity(std::string name)
{
	m_activeAbility = NULL;

	m_name = name;
	hp_base = hp_curr = 100;

	int_base = int_curr = 10;
	str_base = str_curr = 10;
	agi_base = agi_curr = 10;

	m_statsMap["hp_base"] = &hp_base;
	m_statsMap["hp_curr"] = &hp_curr;
	m_statsMap["int_base"] = &int_base;
	m_statsMap["int_curr"] = &int_curr;
	m_statsMap["str_base"] = &str_base;
	m_statsMap["str_curr"] = &str_curr;
	m_statsMap["agi_base"] = &agi_base;
	m_statsMap["agi_curr"] = &agi_curr;
}

GameEntity::~GameEntity(void)
{
	for( int i=0; i< m_abilities.size(); i++)
	{
		m_abilities[i]->release();
	}
}

void GameEntity::addAbility( CastCommandModel* ability )
{
	CastCommandState* state = new CastCommandState(ability, this);
	state->retain();
	m_abilities.push_back(state);
}

std::vector<CastCommandState*>& GameEntity::getAbilityList()
{
	return m_abilities;
}

void GameEntity::setPropertyF( std::string propName, float value )
{

}
float* GameEntity::getPropertyF( std::string propName )
{
	return NULL;
}

void GameEntity::setPropertyI( std::string propName, int value )
{
	if( m_statsMap.count(propName) == 0 ) return;
	(*m_statsMap[propName]) = value;
}
int* GameEntity::getPropertyI( std::string propName )
{
	if( m_statsMap.count(propName) == 0 ) return NULL;
	return m_statsMap[propName];
}
