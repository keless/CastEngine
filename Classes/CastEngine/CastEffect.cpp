
#include "CastEffect.h"

#include "CastEntity.h"

#include "CastCommandScheduler.h"

#include "CastCommandTime.h"

typedef std::string String;

CastEffect::CastEffect()
{
	m_type = CET_DAMAGE_STAT;
	m_startTime = 0;
	m_lifeTime = 0;
	m_numTicksCompleted = 0;
	m_value = 0;
	m_tickFreq = CAST_EFFECT_TICK_FREQ;

	m_pTarget = NULL;
	m_pOrigin = NULL;
	
	this->autorelease();
}

CastEffect::~CastEffect()
{
	//unschedule any callbacks
	cancelTicks();

	CCLOG("~CastEffect");
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

	m_tickFreq = json.get("tickFreq", m_tickFreq).asDouble();
}

bool CastEffect::isPositiveEffect()
{
	if( m_type == CET_BUFF_STAT || m_type == CET_HEAL_STAT)
	{
		return true;
	}
	
	return false;
}

void CastEffect::setTarget( ICastEntity* target )
{
	m_pTarget = target;
}

void CastEffect::startTicks()
{
	m_numTicksCompleted = 0;
	int numTicks = (m_lifeTime / m_tickFreq) + 1;
	CastCommandScheduler::get()->scheduleSelector( schedule_selector(CastEffect::onTick), this, m_tickFreq, numTicks, 0.0f, false);
}

void CastEffect::onTick( float dt )
{
	double currTime = CastCommandTime::get();
	float delta = currTime - m_startTime;
	if( delta > m_lifeTime ) delta = m_lifeTime;
	int numTicksPassed = delta / m_tickFreq;

	int ticksToDo = numTicksPassed - m_numTicksCompleted;
	for( int i=0; i< ticksToDo; i++) {
		doEffect();
		m_numTicksCompleted++;
	}

	if( delta >= m_lifeTime ) 
	{
		//NOTE: scheduleSelectors add references to objects, so we have to 
		// cancel our ticks before the object will be cleaned up by autopool
		cancelTicks(); 
		m_pTarget->removeEffect(this);
	}
}

void CastEffect::cancelTicks()
{
	CastCommandScheduler::get()->unscheduleSelector( schedule_selector(CastEffect::onTick), this );
}

void CastEffect::doEffect()
{
	CCLog("on tick %d", m_numTicksCompleted);
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

	if( m_lifeTime == 0 ) {
		CCLog("instant effect complete-- TODO: clean up");
	}

}

CastEffect* CastEffect::clone()
{
	CastEffect* effect = new CastEffect();


	effect->m_type = m_type;
	effect->m_startTime = m_startTime;
	effect->m_lifeTime = m_lifeTime;
	effect->m_tickFreq = m_tickFreq;
	effect->m_damageType = m_damageType;
	effect->m_targetStat = m_targetStat;
	effect->m_stackFlag = m_stackFlag;
	effect->m_numTicksCompleted = m_numTicksCompleted;
	effect->m_value = m_value;
	effect->m_pTarget = m_pTarget;
	effect->m_pOrigin = m_pOrigin;

	return effect;
}