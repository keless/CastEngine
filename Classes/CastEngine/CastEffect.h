#ifndef _CASTEFFECT_H_
#define _CASTEFFECT_H_


#include <cocos2d.h>
using namespace cocos2d;

#include "json.h"

enum CastEffectType
{
	CET_DAMAGE_STAT,	//decrements stats that can be decremented (health, mana, etc)
	CET_SUPPRESS_STAT,	//temporarily decrements stats while in effect (str, agi, etc)
	CET_HEAL_STAT,		//increments stats that can be incremented
	CET_BUFF_STAT,		//temporarily increases stats while in effect
};

class ICastEntity;

class CastEffect : public CCObject
{

	//time started, time lasting

	CastEffectType m_type;
	double m_startTime;
	double m_lifeTime; //0.0f if instant effect

	std::string m_damageType;
	std::string m_targetStat;
	std::string m_stackFlag; //helper for stacking multiple effects

	int m_numTicksCompleted;

	float m_value; //how much damage, or how much suppression, or how much buff, etc

	//TODO: FIX LIKELY CRASH BUG: when pointers to entities go out of scope
	ICastEntity* m_pTarget;
	ICastEntity* m_pOrigin;

public:
	CastEffect();
	virtual ~CastEffect();

	void init( Json::Value effectData, ICastEntity* from  );
	void setTarget( ICastEntity* to );

	double getLifeTime() { return m_lifeTime; }
	double getElapsedTime( double currTime ) { return currTime - m_lifeTime; } 

	void onTick();

	CastEffect* clone();

	friend class CastWorldModel;
};

#endif
