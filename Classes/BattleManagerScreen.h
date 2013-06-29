#ifndef _BATTLEMANAGER_H_
#define _BATTLEMANAGER_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "CastPhysics.h"

#include "ZZProgressBar.h"
#include "GameEntity.h"
#include "GameEntityView.h"
#include "PlayerView.h"

#include "CastCommandState.h"
#include "CastCommandModel.h"
#include "CastWorldModel.h"

struct EntityPair
{
	GameEntity* model;
	GameEntityView* view;
};


class BattleManagerScreen :
	public CCLayer, public ICastPhysics
{
protected:
	std::vector<EntityPair> m_allEntities;
	std::vector<EntityPair> m_players;
	std::vector<EntityPair> m_enemies;

	std::map<std::string, CastCommandModel*> m_abilities;

	Json::Value readFileToJson( const char* fileName );
	void initAbilities();
	void spawnEnemy();
	void enemyMovementAI( int enemyIdx, float dt );

	float m_travelProgess;
	float m_travelDistance;
	float m_travelLastSpawnLocation;
	ZZProgressBar* m_pbTravel;

	void removeEntity( GameEntity* entity, bool isEnemy );

	float getPartySpeed();

public:
	BattleManagerScreen(void);
	~BattleManagerScreen(void);

	static BattleManagerScreen* create();

	virtual bool init();  

	virtual void update( float dt );

	void PerformEnemyAI( GameEntity* enemy );
	void PerformPlayerAi( GameEntity* player );

	void onEntityEffectEvent( CCObject* e );
	void onEntityDeath( CCObject* e );
	void onEntityLevelup( CCObject* e );
	void setCardDeath( GameEntityView* view );


	virtual bool GetVecBetween( ICastEntity* from, ICastEntity* to, kmVec2& distVec );
	virtual bool GetEntityPosition( ICastEntity* entity, kmVec2& pos );
	virtual bool GetEntitiesInRadius( kmVec2 p, float r, std::vector<ICastEntity*>& entities );

	GameEntityView* getViewForEntity( ICastEntity* entity );

};

#endif
