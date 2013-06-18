#include "GameEntity.h"

#include "CastWorldModel.h"
#include "CastCommandTime.h"

GameEntity::GameEntity(std::string name)
{
	m_name = name;
	hp_base = hp_curr = 100;

	mana_base = mana_curr = 100;

	xp_level = 1;
	xp_next = 0;
	xp_curr = 0;

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
	m_statsMap["xp_curr"] = &xp_curr;
	m_statsMap["xp_next"] = &xp_next;
	m_statsMap["xp_level"] = &xp_level;
	m_statsMap["xp_worth"] = &xp_curr;

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

std::string GameEntity::getLevelStr()
{
	char buff[128];
	snprintf(buff, 128, "%d", xp_level);
	return buff;
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

void GameEntity::setProperty( std::string propName, float value, CastEffect* effect )
{
	if( m_statsMap.count(propName) == 0 ) return;
	(*m_statsMap[propName]) = value;

	dispatch("incProperty", new GameEntityPropertyChangeEvt(propName, value ));
}

void GameEntity::incProperty( std::string propName, float value, CastEffect* effect )
{
	if( m_statsMap.count(propName) == 0 ) return;
	(*m_statsMap[propName]) += value;

	//
	if( propName.compare("hp_base") == 0 ) {
		if( hp_curr > hp_base ) {
			hp_curr = hp_base; //bound hp_curr to max
		}
	}
	
	//bounds check hp_curr
	if( propName.compare("hp_curr") == 0 ) {
		if( hp_curr < 0 ) hp_curr = 0;
		if( hp_curr > hp_base ) hp_curr = hp_base;

		if( hp_curr == 0 ) {
			
			GameEntity* killer = NULL;
			if( CastWorldModel::get()->isValid( effect->getOrigin() ) ) {
				killer = dynamic_cast<GameEntity*>( effect->getOrigin() );
			}
			CCLog("%s: I am slain, by %s!", m_name.c_str(), killer?killer->getName().c_str():"a ghost");

			GameEntityDeathEvt* evt = new GameEntityDeathEvt(killer, this);
			ZZEventBus::game()->dispatch("GameEntityDeathEvt", evt);
		}
	}else if( xp_next != 0 &&  propName.compare("xp_curr") == 0  ) {
		if( xp_curr >= xp_next ) {
			CCLOG("player level up " );

			xp_curr -= xp_next;
			//TODO: figure out what the xp_next value should change to
			xp_next *= 2;
			xp_level++;

			GameEntityLevelupEvt* evt = new GameEntityLevelupEvt(this);
			ZZEventBus::game()->dispatch("GameEntityLevelupEvt", evt);


		}
	}

	CCLog("stat %s delta %0.2f", propName.c_str(), value);

	
	dispatch("incProperty", new GameEntityPropertyChangeEvt(propName, value ));


	//TODO -- if HP <= 0 do logic (set dirty flag and check in update loop)
}

void GameEntity::startBuffProperty( std::string propName, float value, CastEffect* buff)
{
	if( m_statsMap.count(propName) == 0 ) return;
	(*m_statsMap[propName]) += value;
	
	//CCLOG("start buff %s - %f", propName.c_str(), CastCommandTime::get() );

	if( propName.compare("hp_base") == 0 ) {
		if( hp_base < 0 ) hp_base = 0;
		if( hp_curr < hp_base ) hp_curr = hp_base;
	}

}
void GameEntity::endBuffProperty( std::string propName, float value, CastEffect* buff)
{
	if( m_statsMap.count(propName) == 0 ) return;


	(*m_statsMap[propName]) += value;

	//CCLOG("end buff %s - %f", propName.c_str(), CastCommandTime::get() );

	if( propName.compare("hp_base") == 0 ) {

		if( value > 0 ) {
			//returning max health

		}else {
			//losing max health
			if( hp_base < 0 ) hp_base = 0;
			if( hp_curr < hp_base ) hp_curr = hp_base;
		}

		
		
	}
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

void GameEntity::applyEffect( CastEffect* effect )
{
	
	if( effect->getLifeTime() == 0 ) 
	{
		//CCLog("apply instant effect");

		effect->doEffect();

	}
	else {
		//CCLog("apply effect over time");

		if( effect->getType() == CET_BUFF_STAT ) 
		{
			m_buffs[effect] = effect;
		}else if( effect->getType() == CET_SUPPRESS_STAT )
		{
			m_debuffs[effect] = effect;
		}
		else if( effect->getType() == CET_DAMAGE_STAT )
		{
			m_negativeEffects.push_back(effect);
		}else {
			m_positiveEffects.push_back(effect);
		}

		
		effect->retain();
		effect->startTicks();
	}


 }

void GameEntity::removeEffect( CastEffect* effect )
{
	
	if( effect->getType() == CET_BUFF_STAT ) 
	{
		m_debuffs.erase( effect );
	}
	else if( effect->getType() == CET_SUPPRESS_STAT )
	{
		m_debuffs.erase( effect );
	}
	else if( effect->getType() == CET_DAMAGE_STAT )
	{
		for( int i=0; i< m_negativeEffects.size(); i++ )
		{
			if( m_negativeEffects[i] == effect ) {
				m_negativeEffects.erase( m_negativeEffects.begin() + i );
				return;
			}
		}
	}else {
		for( int i=0; i< m_positiveEffects.size(); i++ )
		{
			if( m_positiveEffects[i] == effect ) {
				m_positiveEffects.erase( m_positiveEffects.begin() + i );
				return;
			}
		}
	}

	CC_SAFE_RELEASE( effect );





}
