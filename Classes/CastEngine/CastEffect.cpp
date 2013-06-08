
#include "CastEffect.h"

#include "CastEntity.h"

#include "CastCommandScheduler.h"

#include "CastCommandTime.h"

#include "CastWorldModel.h"

#include "CastCommandState.h"

#include "CastCommandModel.h"

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

	m_pModel = NULL;
	m_modelEffectIndex = -1;
	
	this->autorelease();
}

CastEffect::~CastEffect()
{
	CC_SAFE_RELEASE_NULL(m_pModel);

	//unschedule any callbacks
	cancelTicks();

	CCLOG("~CastEffect");
}

void CastEffect::init(  CastCommandState* originState, int effectIdx, ICastEntity* from  )
{
	m_pModel = originState->m_pModel;
	m_pModel->retain();
	m_modelEffectIndex = effectIdx;

	Json::Value& json = m_pModel->descriptor["effectsOnCast"].get(effectIdx, Json::Value());

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
	if( !CastWorldModel::get()->isValid( target ) ) return;
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
	if( !CastWorldModel::get()->isValid( m_pTarget ) ) return;

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
	if( !CastWorldModel::get()->isValid( m_pTarget ) ) return;

	Json::Value json = m_pModel->getEffectOnCast(m_modelEffectIndex);
	if( json.isMember("react") ) {
		m_pTarget->handleEffectReaction( json["react"] );
	}
	
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

Json::Value CastEffect::getDescriptor( std::string name )
{
	if( m_pModel == NULL || m_modelEffectIndex < 0 ) return Json::Value();

	if( name.size() == 0 )
		return	m_pModel->descriptor[m_modelEffectIndex];
	
	return m_pModel->descriptor[m_modelEffectIndex].get(name, Json::Value() );
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
	effect->m_pModel = m_pModel;
	effect->m_modelEffectIndex = m_modelEffectIndex;

	return effect;
}