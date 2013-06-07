#ifndef _GAMEENTITY_H_
#define _GAMEENTITY_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "CastEntity.h"
#include "CastCommandState.h"
#include "CastTarget.h"

#include "ZZEventBus.h"



class GameEntity :
	public CCObject, public ICastEntity, public ZZEventBus
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

	CastTarget* m_abilityTargets;

	std::vector<CastEffect*> m_negativeEffects;
	std::vector<CastEffect*> m_positiveEffects;

	bool m_isDirty; //every frame: true if stat changed, false if not

public:
	GameEntity( std::string name );
	~GameEntity(void);
	
	std::string getName() { return m_name; }

	bool isDirty() { return m_isDirty; }
	void setDirty( bool dirt ) { m_isDirty = dirt; }

	virtual void addAbility( CastCommandModel* ability );
	std::vector<CastCommandState*>& getAbilityList();

	//ICastEntity methods
	virtual void incProperty( std::string propName, float value );
	virtual float getProperty( std::string propName );

	virtual CastTarget* getTarget();
	virtual void sendEffectToTarget( CastEffect* effect, float speed ); //effect LEAVING from this entity
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

#endif
