#ifndef _CASTOBJECT_H_
#define _CASTOBJECT_H_

#include <string>
#include <vector>

class CastTarget;
class CastEffect;
class CastEntityPointer;

class ICastEntity 
{
private:
	std::vector<CastEntityPointer*> m_CastPointers;
	void detachAllPointers();
	void attachPointer( CastEntityPointer* ptr );
	void detachPointer( CastEntityPointer* ptr );
	friend class CastEntityPointer;

public:

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
