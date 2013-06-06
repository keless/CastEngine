#include "CastCommandState.h"

#include "CastCommandTime.h"

#include "CastCommandScheduler.h"

#include "CastEffect.h"

CastCommandState::CastCommandState(CastCommandModel* model, ICastEntity* owner)
{
	m_iOwner = owner;
	m_pModel = model;
	m_pModel->retain();

	m_state = CCS_IDLE;
	m_timeStart = 0;
}


CastCommandState::~CastCommandState(void)
{
	m_pModel->release();
}

float CastCommandState::getCastPct()
{
	if( m_state == CCS_CASTING ) {
		double currTime = CastCommandTime::get();
		float delta = (float)(currTime - m_timeStart);
		if( delta > m_pModel->castTime ) delta = m_pModel->castTime;  //TODO: support cast speed increase
		if( delta < 0 ) delta = 0;
		return delta / m_pModel->castTime;
	}else {
		return 1.0f;
	}
}

float CastCommandState::getCooldownPct()
{

	if( m_state == CCS_COOLDOWN ) {
		double currTime = CastCommandTime::get();
		float delta = (float)(currTime - m_timeStart);
		if( delta > m_pModel->cooldownTime ) delta = m_pModel->cooldownTime; //TODO: support cooldown decrease
		if( delta < 0 ) delta = 0;
		return delta / m_pModel->cooldownTime;
	}else {
		return 1.0f;
	}
}

bool CastCommandState::startCast()
{
	if( ! isIdle() ) return false;

	m_state = CCS_CASTING;
	m_timeStart = CastCommandTime::get();

	if( m_pModel->castTime == 0 ) {
		//handle instant cast
		onSchedulerTick(0.0f);

	}else {

		//TODO: should we set castTime as delay instead of interval?
		CastCommandScheduler::get()->scheduleSelector( schedule_selector(CastCommandState::onSchedulerTick), this, m_pModel->castTime, 0, 0.0f, false);
	}

	return true;
}

void CastCommandState::onSchedulerTick( float dt)
{
	

	double currTime = CastCommandTime::get();
	float delta = (float)(currTime - m_timeStart);

	CCLog("Cast %s tick : delta %.2f", m_pModel->getName().c_str(), delta); 

	if( m_state == CCS_CASTING ) 
	{
		if( delta >= m_pModel->castTime )  //TODO: handle cast speed increase
		{ 
			//casting complete
			//TODO: handle extra time lost if delta > castTime?

			CastCommandScheduler::get()->unscheduleSelector( schedule_selector(CastCommandState::onSchedulerTick), this );
			onCastComplete();
		}
	}
	else if( m_state == CCS_CHANNELING ) {
		//TODO: handle channeling ticks
	}
	else if( m_state == CCS_COOLDOWN ) 
	{
		if( delta >= m_pModel->cooldownTime ) //TODO: handle cooldown redux
		{
			CastCommandScheduler::get()->unscheduleSelector( schedule_selector(CastCommandState::onSchedulerTick), this );
			onCooldownComplete();
		}
	}
}

void CastCommandState::onCastComplete()
{
	if( m_state != CCS_CASTING ) return;

	double currTime = CastCommandTime::get();
	m_timeStart = currTime;
	
	//TODO: spawn effects
	CastTarget* target = m_iOwner->getTarget();
	for( int i=0; i< m_pModel->effectsOnCast.size(); i++ )
	{
		CastEffect* effect = new CastEffect();
		
		//TODO: send all effects as one array so only one "packet" has to travel?
		m_iOwner->sendEffectToTarget( effect, m_pModel->travelSpeed );
		
	}

	if( m_pModel->channelTime > 0.0f ) {
		//begin channeling
		//TODO: spawn channeling samples
		m_state = CCS_CHANNELING;

	}else {
		CCLog("Cast %s complete!", m_pModel->getName().c_str());

		m_state = CCS_COOLDOWN; //TODO: handle cooldown redux?
		CastCommandScheduler::get()->scheduleSelector( schedule_selector(CastCommandState::onSchedulerTick), this, m_pModel->cooldownTime, 0, 0.0f, false);
	}
}

void CastCommandState::onCooldownComplete()
{
	if( m_state != CCS_COOLDOWN ) return;

	CCLog("Cooldown %s complete!",  m_pModel->getName().c_str());

	double currTime = CastCommandTime::get();
	m_timeStart = currTime;

	m_state = CCS_IDLE;

	//TODO: send cooldown complete signal
}
