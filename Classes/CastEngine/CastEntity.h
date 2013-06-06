#ifndef _CASTOBJECT_H_
#define _CASTOBJECT_H_

#include <string>

class CastTarget;
class CastEffect;

class ICastEntity 
{
public:

	virtual void setPropertyF( std::string propName, float value ) {}
	virtual float* getPropertyF( std::string propName ) { return NULL; }
	virtual void setPropertyI( std::string propName, int value ) {}
	virtual int* getPropertyI( std::string propName )  { return NULL; }

	CastTarget* getTarget() { return NULL; }
	
	//speed == 0.0f means no travel time, instant effect
	void sendEffectToTarget( CastEffect* effect, float speed ) {}
};

#endif
