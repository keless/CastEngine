#ifndef _CASTEFFECT_H_
#define _CASTEFFECT_H_


#include <cocos2d.h>
using namespace cocos2d;

class CastEffect : public CCObject
{

	//time started, time lasting
	// array of stats to effect?

	double startTime;
	double lifeTime; //0.0f if instant effect

public:
	CastEffect();
};

#endif
