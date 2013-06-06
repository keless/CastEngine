
#include "CastEffect.h"

#include "CastEntity.h"

typedef std::string String;

CastEffect::CastEffect()
{
	m_type = CET_DAMAGE_STAT;
	m_startTime = 0;
	m_lifeTime = 0;
	m_numTicksCompleted = 0;
	m_value = 0;

	m_pTarget = NULL;
	m_pOrigin = NULL;
	
	this->autorelease();
}

CastEffect::~CastEffect()
{
	//unschedule any callbacks?
}

void CastEffect::init( Json::Value json, ICastEntity* from )
{
	String type = json.get("effectType", "damage").asString();
	if( type.compare("damage") == 0 ){
		m_type = CET_DAMAGE_STAT;
	}

	m_pOrigin = from;

	m_damageType = json.get("damageType", "").asString(); //ex: fire
	m_targetStat = json.get("targetStat", "").asString(); //ex: hp_curr

	m_value = json.get("valueBase", 0.0f).asDouble(); //ex: 10 damage
	//todo: handle caster stat modifiers

	m_lifeTime = json.get("effectLifetime", 0.0f).asDouble(); //ex: 1.0 seconds
}

void CastEffect::setTarget( ICastEntity* target )
{
	m_pTarget = target;
}

void CastEffect::onTick()
{
	CCLog("todo: on tick");
	switch( m_type ) 
	{
	case CET_DAMAGE_STAT:
		m_pTarget->incProperty( m_targetStat, -1* m_value );
		break;

	case CET_HEAL_STAT:
		m_pTarget->incProperty( m_targetStat,  m_value );
		break;

	default:
		CCLOG("TODO: handle effect type");
	}
	
}

CastEffect* CastEffect::clone()
{
	CastEffect* effect = new CastEffect();


	effect->m_type = m_type;
	effect->m_startTime = m_startTime;
	effect->m_lifeTime = m_lifeTime;
	effect->m_numTicksCompleted = m_numTicksCompleted;
	effect->m_value = m_value;

	return effect;
}