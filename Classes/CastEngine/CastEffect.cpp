
#include "CastEffect.h"

CastEffect::CastEffect()
{
	m_type = CET_DAMAGE_STAT;
	m_startTime = 0;
	m_lifeTime = 0;
	m_numTicksCompleted = 0;
	m_value = 0;
}

CastEffect::~CastEffect()
{
	//unschedule any callbacks?
}

void CastEffect::onTick()
{

}