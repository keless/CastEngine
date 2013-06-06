#ifndef _GAMEENTITY_H_
#define _GAMEENTITY_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "CastEntity.h"
#include "CastCommandState.h"
#include "CastTarget.h"

class GameEntity :
	public CCObject, public ICastEntity
{
	int hp_base;
	int hp_curr;

	int int_base;
	int int_curr;

	int str_base;
	int str_curr;

	int agi_base;
	int agi_curr;

	std::map<std::string, int*> m_statsMap;

	std::string m_name;

	std::vector<CastCommandState*> m_abilities;
	CastCommandState* m_activeAbility;

	CastTarget* m_abilityTargets;

	std::vector<CastEffect*> m_negativeEffects;
	std::vector<CastEffect*> m_positiveEffects;

public:
	GameEntity( std::string name );
	~GameEntity(void);
	
	std::string getName() { return m_name; }

	virtual void addAbility( CastCommandModel* ability );
	std::vector<CastCommandState*>& getAbilityList();

	//ICastEntity methods
	virtual void incProperty( std::string propName, float value );
	virtual float getProperty( std::string propName );

	virtual CastTarget* getTarget();
	virtual void sendEffectToTarget( CastEffect* effect, float speed ); //effect LEAVING from this entity
	virtual void applyEffect( CastEffect* effect );						//effect ARRIVING at this entity

	

};


#endif
