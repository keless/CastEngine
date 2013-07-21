#ifndef _GAMEENTITY_H_
#define _GAMEENTITY_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "CastEntity.h"
#include "CastEffect.h"
#include "CastCommandState.h"
#include "CastTarget.h"

#include "ZZEventBus.h"
using namespace ZZ;


class GameEntity :
	public CCObject, public ICastEntity, public EventBus
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

	int xp_level;
	int xp_curr;
	int xp_next;

	std::map<std::string, int*> m_statsMap;

	std::string m_name;

	std::vector<CastCommandState*> m_abilities;

	CastTarget* m_abilityTargets;

	std::vector<CastEffect*> m_negativeEffects;
	std::vector<CastEffect*> m_positiveEffects;

	std::map< std::string , CastEffect*> m_buffs;
	std::map< std::string , CastEffect*> m_debuffs;

public:
	GameEntity( std::string name );
	~GameEntity(void);

	Json::Value toJson();
	void initFromJson( const Json::Value& json );
	
	std::string getName() { return m_name; }
	void setName(std::string name) { m_name = name; }

	std::string getLevelStr();

	virtual void addAbility( CastCommandModel* ability );
	std::vector<CastCommandState*>& getAbilityList();
	
	float getDebuffTimeLeft( std::string debufName );
	int numDebuffs() { return m_debuffs.size(); }
	int numBuffs() { return m_buffs.size(); }

	//true if not casting any abilities and has valid target
	bool canCast();

	//ICastEntity methods
	virtual void setProperty( std::string propName, float value, CastEffect* effect );
	virtual void incProperty( std::string propName, float value, CastEffect* effect );
	virtual float getProperty( std::string propName );
	virtual void  startBuffProperty( std::string propName, float value, CastEffect* buff);
	virtual void  endBuffProperty( std::string propName, float value, CastEffect* buff);

	virtual void handleEffectReaction( Json::Value& reaction, CastEffect* source );
	virtual void handleEffectEvent( std::string effectEventName, CastEffect* source );

	virtual CastTarget* getTarget();
	virtual void applyEffect( CastEffect* effect );						//effect ARRIVING at this entity
	virtual void removeEffect( CastEffect* effect );
	

};

//dispatches from the game entity
class GameEntityPropertyChangeEvt  : public CCObject
{
public:
	std::string prop;
	float delta;

	GameEntityPropertyChangeEvt(std::string propertyName, float valueChange ) : prop(propertyName), delta(valueChange) { this->autorelease(); }
};

//dispatches from the game entity
class GameEntityReactEvt : public CCObject 
{
public:
	Json::Value react;
	CastEffect* source;

	GameEntityReactEvt( Json::Value reaction, CastEffect* src ) : react(reaction), source(src) { this->autorelease(); }
};


//NOTE: dispatches on global game bus, not the GameEntity
class GameEntityDeathEvt : public CCObject
{
public:
	ICastEntity* killer;
	ICastEntity* killed;

	GameEntityDeathEvt( ICastEntity* kill_origin, ICastEntity* dead_entity ) : killer(kill_origin), killed(dead_entity) { this->autorelease(); }
};

//NOTE: dispatches on global game bus, not the GameEntity
class GameEntityLevelupEvt : public CCObject 
{
public:
	ICastEntity* target;

	GameEntityLevelupEvt( ICastEntity* levelee ) : target(levelee) { this->autorelease(); }
};

//NOTE: dispatches on global game bus, not the GameEntity
class GameEntityEffectEvt : public CCObject
{
public:
	ICastEntity* target;
	ICastEntity* origin;
	std::string name;

	GameEntityEffectEvt( std::string effect, ICastEntity* from, ICastEntity* to ) : name(effect), target(to), origin(from) { this->autorelease(); }
};
#endif
