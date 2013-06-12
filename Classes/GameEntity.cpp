#include "GameEntity.h"

#include "CastWorldModel.h"
#include "CastCommandTime.h"

GameEntity::GameEntity(std::string name)
{
	m_name = name;
	hp_base = hp_curr = 100;

	mana_base = mana_curr = 100;

	int_base = int_curr = 10;
	str_base = str_curr = 10;
	agi_base = agi_curr = 10;

	m_statsMap["hp_base"] = &hp_base;
	m_statsMap["hp_curr"] = &hp_curr;
	m_statsMap["mana_base"] = &mana_base;
	m_statsMap["mana_curr"] = &mana_curr;
	m_statsMap["int_base"] = &int_base;
	m_statsMap["int_curr"] = &int_curr;
	m_statsMap["str_base"] = &str_base;
	m_statsMap["str_curr"] = &str_curr;
	m_statsMap["agi_base"] = &agi_base;
	m_statsMap["agi_curr"] = &agi_curr;

	m_abilityTargets = new CastTarget();
}

GameEntity::~GameEntity(void)
{
	CC_SAFE_RELEASE_NULL( m_abilityTargets );

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

bool GameEntity::canCast()
{
	//dont send touch to abilities if any ability is casting
	for( int i=0; i< m_abilities.size(); i++) {
		if( m_abilities[i]->isCasting() ) {
			//abort touch
			return false;
		}
	}

	if(! m_abilityTargets->hasValidTarget() ) return false;

	return true;
}

void GameEntity::incProperty( std::string propName, float value )
{
	if( m_statsMap.count(propName) == 0 ) return;
	(*m_statsMap[propName]) += value;

	//bounds check hp_curr
	if( propName.compare("hp_curr") == 0 ) {
		if( hp_curr < 0 ) hp_curr = 0;
		if( hp_curr > hp_base ) hp_curr = hp_base;
	}

	CCLog("stat %s delta %0.2f", propName.c_str(), value);

	
	dispatch("incProperty", new GameEntityPropertyChangeEvt(propName, value ));


	//TODO -- if HP <= 0 do logic (set dirty flag and check in update loop)
}
float GameEntity::getProperty( std::string propName )
{
	if( m_statsMap.count(propName) == 0 ) return 0;
	return (*m_statsMap[propName]);
}

void GameEntity::handleEffectReaction( Json::Value& reaction, CastEffect* source )
{
	GameEntityReactEvt* evt = new GameEntityReactEvt( reaction, source );
	dispatch("react", evt);
}

CastTarget* GameEntity::getTarget()
{
	return m_abilityTargets;
}

void GameEntity::sendEffectToTarget( CastEffect* effect )
{
	CastWorldModel* world = CastWorldModel::get();
	
	//TODO: handle ammo/mana cost?
	
	world->addEffectInTransit(this, effect, m_abilityTargets, CastCommandTime::get());
	
	
}

void GameEntity::applyEffect( CastEffect* effect )
{
	
	if( effect->getLifeTime() == 0 ) 
	{
		CCLog("apply instant effect");

		effect->doEffect();

	}
	else {
		CCLog("todo: apply effect over time");

		m_negativeEffects.push_back(effect);
		effect->retain();
		effect->startTicks();
	}


 }

void GameEntity::removeEffect( CastEffect* effect )
{
	for( int i=0; i< m_negativeEffects.size(); i++ )
	{
		if( m_negativeEffects[i] == effect ) {
			m_negativeEffects[i]->release();
			m_negativeEffects.erase( m_negativeEffects.begin() + i );
			return;
		}
	}

	for( int i=0; i< m_positiveEffects.size(); i++ )
	{
		if( m_positiveEffects[i] == effect ) {
			m_positiveEffects[i]->release();
			m_positiveEffects.erase( m_positiveEffects.begin() + i );
			return;
		}
	}
}
