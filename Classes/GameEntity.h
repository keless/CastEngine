#ifndef _GAMEENTITY_H_
#define _GAMEENTITY_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "CastEntity.h"
#include "CastEffect.h"
#include "CastCommandState.h"
#include "CastTarget.h"

#include "ZZEventBus.h"



class GameEntity :
	public CCObject, public ICastEntity, public ZZEventBus
{
	int hp_base;
	int hp_curr;

	int mana_base;
	int mana_curr;

	int int_base;
	int int_curr;

	int str_base;
	int str_curr;

	int agi_base;
	int agi_curr;

	std::map<std::string, int*> m_statsMap;

	std::string m_name;

	std::vector<CastCommandState*> m_abilities;

	CastTarget* m_abilityTargets;

	std::vector<CastEffect*> m_negativeEffects;
	std::vector<CastEffect*> m_positiveEffects;

	std::map<CastEffect*, CastEffect*> m_buffs;
	std::map<CastEffect*, CastEffect*> m_debuffs;

public:
	GameEntity( std::string name );
	~GameEntity(void);
	
	std::string getName() { return m_name; }

	virtual void addAbility( CastCommandModel* ability );
	std::vector<CastCommandState*>& getAbilityList();

	//true if not casting any abilities and has valid target
	bool canCast();

	//ICastEntity methods
	virtual void incProperty( std::string propName, float value );
	virtual float getProperty( std::string propName );
	virtual void  startBuffProperty( std::string propName, float value, CastEffect* buff);
	virtual void  endBuffProperty( std::string propName, float value, CastEffect* buff);

	virtual void handleEffectReaction( Json::Value& reaction, CastEffect* source );

	virtual CastTarget* getTarget();
	virtual void sendEffectToTarget( CastEffect* effect ); //effect LEAVING from this entity
	virtual void applyEffect( CastEffect* effect );						//effect ARRIVING at this entity
	virtual void removeEffect( CastEffect* effect );
	

};

class GameEntityPropertyChangeEvt  : public CCObject
{
public:
	std::string prop;
	float delta;

	GameEntityPropertyChangeEvt(std::string propertyName, float valueChange ) : prop(propertyName), delta(valueChange) { this->autorelease(); }
};

class GameEntityReactEvt : public CCObject 
{
public:
	Json::Value react;
	CastEffect* source;

	GameEntityReactEvt( Json::Value reaction, CastEffect* src ) : react(reaction), source(src) { this->autorelease(); }
};

#endif
