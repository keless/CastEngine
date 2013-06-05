#ifndef _CASTEFFECT_H_
#define _CASTEFFECT_H_


#include <cocos2d.h>
using namespace cocos2d;

enum CastEffectType
{
	CET_DAMAGE_STAT,	//decrements stats that can be decremented (health, mana, etc)
	CET_SUPPRESS_STAT,	//temporarily decrements stats while in effect (str, agi, etc)
	CET_BUFF_STAT,		//temporarily increases stats while in effect
};

class CastEffect : public CCObject
{

	//time started, time lasting

	CastEffectType m_type;
	double m_startTime;
	double m_lifeTime; //0.0f if instant effect

	int m_numTicksCompleted;

	float m_value; //how much damage, or how much suppression, or how much buff, etc

public:
	CastEffect();
	virtual ~CastEffect();

	void onTick();
};

#endif
