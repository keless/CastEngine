#ifndef _CASTPOINTER_H_
#define _CASTPOINTER_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "CastEntity.h"

class CastEntityPointer :
	public CCObject, public ICastEntity
{
	ICastEntity* m_ptr;

public:
	CastEntityPointer( ICastEntity* entity )
	{
		m_ptr = entity;
		m_ptr->attachPointer(this);
	}
	~CastEntityPointer(void)
	{
		if( m_ptr != NULL ) 
			m_ptr->detachPointer(this);
	}

	bool isPtrValid() { return m_ptr != NULL; }


	virtual void  incProperty( std::string propName, float value ) {
		if(m_ptr == NULL) return;
		m_ptr->incProperty(propName, value);
	}
	virtual float getProperty( std::string propName ) { 
		if( m_ptr == NULL ) return 0; 
		return m_ptr->getProperty(propName);
	}

	virtual CastTarget* getTarget() { 
		if(m_ptr == NULL) return NULL; 
		return m_ptr->getTarget();
	}
	
	//effect is LEAVING this entity, towards somewhere else
	virtual void sendEffectToTarget( CastEffect* effect, float speed ) {
		if( m_ptr == NULL ) return;
		m_ptr->sendEffectToTarget(effect, speed);
	}

	//effect is ARRIVING at this entity
	virtual void applyEffect( CastEffect* effect ) {
		if( m_ptr == NULL ) return;
		m_ptr->applyEffect(effect);
	}
	virtual void removeEffect( CastEffect* effect )	{
		if( m_ptr == NULL ) return;
		m_ptr->removeEffect(effect);
	}
};

#endif