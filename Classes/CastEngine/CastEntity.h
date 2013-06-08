#ifndef _CASTOBJECT_H_
#define _CASTOBJECT_H_

#include <string>

class CastTarget;
class CastEffect;

class ICastEntity 
{
public:

	ICastEntity();
	virtual ~ICastEntity();

	//send negative values to 'decrement'
	virtual void  incProperty( std::string propName, float value ) {}
	virtual float getProperty( std::string propName ) { return 0; }

	virtual CastTarget* getTarget() { return NULL; }
	
	//effect is LEAVING this entity, towards somewhere else
	//speed == 0.0f means no travel time, instant effect
	virtual void sendEffectToTarget( CastEffect* effect, float speed ) = 0;

	//effect is ARRIVING at this entity
	virtual void applyEffect( CastEffect* effect ) = 0;
	virtual void removeEffect( CastEffect* effect ) = 0;
};

#endif
