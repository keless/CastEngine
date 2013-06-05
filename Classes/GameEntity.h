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

public:
	GameEntity( std::string name );
	~GameEntity(void);

	void addAbility( CastCommandModel* ability );
	std::vector<CastCommandState*>& getAbilityList();

	void setPropertyF( std::string propName, float value );
	float* getPropertyF( std::string propName );
	void setPropertyI( std::string propName, int value );
	int* getPropertyI( std::string propName );

	CastTarget* getTarget();

	std::string getName() { return m_name; }

};


#endif
