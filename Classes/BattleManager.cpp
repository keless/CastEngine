#include "BattleManager.h"

#define MAX_STEP 1.0f
#define TIME_SCALE 0.5f
#include "CastCommandTime.h"
#include "CastCommandScheduler.h"
#include "CastWorldModel.h"

#define GAME_UNIT_CONVERSION (1.0f/210.0f)
#define VIEW_UNIT_CONVERSION (210.0f)

//static 
BattleManager* BattleManager::create()
{
	BattleManager* bm = new BattleManager();
	bm->init();
	bm->autorelease();
	return bm;
}

BattleManager::BattleManager(void)
{
	m_travelProgess = 0;
	m_pbTravel = NULL;
}

bool BattleManager::init()
{

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	setContentSize( visibleSize );
	
	initAbilities();

	m_travelLastSpawnLocation = 0.0f;
	m_travelProgess = 0.0f;
	m_travelDistance = 100.0f;
	float pbMargin = 50;
	m_pbTravel = ZZProgressBar::create( CCRectMake( pbMargin, visibleSize.height - pbMargin, visibleSize.width - pbMargin*2, pbMargin ) );
	m_pbTravel->setProgress( m_travelProgess / m_travelDistance );
	addChild(m_pbTravel);

	//todo: remove listener on destructor
	ZZEventBus::game()->addListener("GameEntityDeathEvt", this, callfuncO_selector(BattleManager::onEntityDeath));
	ZZEventBus::game()->addListener("GameEntityLevelupEvt", this, callfuncO_selector(BattleManager::onEntityLevelup));
	ZZEventBus::game()->addListener("GameEntityEffectEvt", this, callfuncO_selector(BattleManager::onEntityEffectEvent));
	
	
	CastWorldModel::get()->setPhysicsInterface(this);



	EntityPair player;

	player.model = new GameEntity("Leeroy");
	//m_playerModel->incProperty("hp_curr", -90);
	player.model->incProperty("xp_next", 100, NULL);
	player.model->addAbility( m_abilities["fireball"] );
	//player.model->addAbility( m_abilities["sword attack"] );
	//player.model->addAbility( m_abilities["Heal"] );
	player.model->addAbility( m_abilities["Life Drain"] );
	player.model->addAbility( m_abilities["Curse of Weakness"] );

	player.view = new GameEntityView( player.model );
	player.view->setPositionX(150);
	player.view->setPositionY(350);
	addChild(player.view);

	m_players.push_back(player);
	m_allEntities.push_back(player);


	player.model = new GameEntity("Derpsan");
	//m_playerModel->incProperty("hp_curr", -90);
	player.model->incProperty("xp_next", 100, NULL);
	//player.model->addAbility( m_abilities["fireball"] );
	player.model->addAbility( m_abilities["sword attack"] );
	player.model->addAbility( m_abilities["Death Grip"] );
	//player.model->addAbility( m_abilities["Life Drain"] );
	//player.model->addAbility( m_abilities["Curse of Weakness"] );

	player.view = new GameEntityView( player.model );
	player.view->setPositionX(150);
	player.view->setPositionY(150);
	addChild(player.view);

	m_players.push_back(player);
	m_allEntities.push_back(player);

	spawnEnemy();

	scheduleUpdate();
	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

BattleManager::~BattleManager(void)
{
	ZZEventBus::game()->remListener("GameEntityDeathEvt", this, callfuncO_selector(BattleManager::onEntityDeath));
	ZZEventBus::game()->remListener("GameEntityLevelupEvt", this, callfuncO_selector(BattleManager::onEntityLevelup));
	ZZEventBus::game()->remListener("GameEntityEffectEvt", this, callfuncO_selector(BattleManager::onEntityEffectEvent));
}

float BattleManager::getPartySpeed()
{
	return 1.0f; //todo: derive from party stats
}

void BattleManager::removeEntity( GameEntity* entity, bool isEnemy )
{
	if( isEnemy ) {
		for( int i=m_enemies.size()-1; i >= 0; i--) {
			EntityPair& enemy = m_enemies[i];
			if( enemy.model == entity ) {
				m_enemies.erase( m_enemies.begin() + i );
				break;
			}
		}
	}else {
		for( int i=m_players.size()-1; i >= 0; i--) {
			EntityPair& enemy = m_players[i];
			if( enemy.model == entity ) {
				m_players.erase( m_players.begin() + i );
				break;
			}
		}

	}

	for( int i=m_allEntities.size()-1; i >= 0; i--) {
		EntityPair& enemy = m_allEntities[i];
		if( enemy.model == entity ) {
			m_allEntities.erase( m_allEntities.begin() + i );
			CC_SAFE_RELEASE_NULL(enemy.model);
			CC_SAFE_RELEASE_NULL(enemy.view);

			break;
		}
	}

}

//virtual 
void BattleManager::update( float dt )
{
	if( dt > MAX_STEP ) dt = MAX_STEP;

	dt *= TIME_SCALE;

	CastCommandTime::updateDelta(dt);
	CastCommandScheduler::get()->update(dt);
	CastWorldModel::get()->updateStep(dt);

	if( m_players.size() < 1 ) 
	{
		//todo: handle game over 
		ZZEventBus::BaseEvent* evt = new ZZEventBus::BaseEvent("mainMenu");
		ZZEventBus::get("state")->dispatch("switchTo", evt);
		return;
	}

	for( int i=m_enemies.size()-1; i >= 0; i--) {
		EntityPair& enemy = m_enemies[i];

		if( enemy.model->getProperty("hp_curr") <= 0 ) {
			setCardDeath(enemy.view);
			//removeChild(enemy.enemyView);

			removeEntity( enemy.model, true );
		}
		else {
			enemyMovementAI(i, dt);

			PerformEnemyAI(enemy.model);
		}
	}


	if( m_enemies.size()  == 0 ) {


		//move forward

		float speed = getPartySpeed();
		m_travelProgess += speed * dt;

		m_pbTravel->setProgress( m_travelProgess/ m_travelDistance );

		if( m_travelProgess >= m_travelLastSpawnLocation + 25 )
		{
			//spawn more
			m_travelLastSpawnLocation = m_travelDistance +  25;
			

			int randNum = 1 + (rand() % 3);
			CCLog("spawn %d enemies", randNum);
			for( int i=0; i< randNum; i++) {
				spawnEnemy();
			}
		

		}

		


		if( m_travelProgess >= m_travelDistance )
		{
			//reached target, do something
			ZZEventBus::get("state")->dispatch("switchTo", new ZZEventBus::BaseEvent("mainMenu") );
			return;
		}
	}
	
	
	for( int i=0; i< m_players.size(); i++)
	{
		EntityPair& player = m_players[i];
		if( player.model->getProperty("hp_curr") <= 0 ) {
			setCardDeath(player.view);
			//removeChild(player.enemyView);

			removeEntity( player.model, true );
		}else {
			PerformPlayerAi(m_players[i].model);
		}

		
	}

	
}

//#define DISABLE_ATTACKS

void BattleManager::PerformEnemyAI( GameEntity* enemy )
{
	//select ability
	std::vector<CastCommandState*> abilities;
	abilities = enemy->getAbilityList();

	CastCommandState* cast = abilities[ rand() % abilities.size() ];

	EntityPair& player = m_players[0];

		//select target
	//TODO: if beneficial, select friendly
	//TODO: handle selection from multiple 'players'
	enemy->getTarget()->clearTargetEntities();
	enemy->getTarget()->addTargetEntity(player.model);

	if( enemy->canCast() ) {

#ifndef DISABLE_ATTACKS
		cast->startCast();
#endif

	}
}

void BattleManager::PerformPlayerAi( GameEntity* player )
{
	//select ability
	std::vector<CastCommandState*> abilities;
	abilities = player->getAbilityList();

	CastCommandState* cast = abilities[ rand() % abilities.size() ];

	ICastEntity* target = NULL;
	if( player->getTarget()->getEntityList().size() > 0 )
		target = player->getTarget()->getEntityList()[0];
	if( !CastWorldModel::get()->isValid( target ) )
	{
		player->getTarget()->clearTargetEntities();

		if( m_enemies.size() > 0 ) {

			//target closest enemy
			target = m_enemies[0].model;
			kmVec2 distVec;
			GetVecBetween(player, target, distVec);
			float closestEnemy = kmVec2Length( &distVec );

			for( int i=1; i< m_enemies.size(); i++)
			{
				GetVecBetween(player, m_enemies[i].model, distVec);
				float distTo = kmVec2Length( &distVec );

				if( distTo < closestEnemy ) {
					closestEnemy = distTo;
					target = m_enemies[i].model;
				}
				
			}

			
			player->getTarget()->addTargetEntity(target);
		}
	}

	kmVec2 toTarget;
	if( GetVecBetween(player, target, toTarget) && player->canCast() && cast->canAfford() ) {
		
		float dTargetSq = kmVec2LengthSq( &toTarget );
		float range = cast->getRange();

		float meleeRange = 1.0f;

		if( range*range >= dTargetSq ) {
		
			bool dontCast = false;
			if( cast->getName().compare("Death Grip") == 0 )
			{
				//abort if in melee range already
				if( dTargetSq <= meleeRange*meleeRange ) {
					dontCast = true;
					//CCLog("target in melee range, abort death grip");
				}
			}
			else if( cast->getName().compare("Curse of Weakness") == 0 ) {
				//abort if already has curse of weakness and time left greater than 2s
				GameEntity* geTarget = dynamic_cast<GameEntity*>(target);
				if( geTarget != NULL ) {
					float lt = geTarget->getDebuffTimeLeft("Curse of Weakness");
					if( lt > 2 ) 
						dontCast = true;
				}
			}

#ifndef DISABLE_ATTACKS
			if( !dontCast ) {
				cast->startCast();
			}
#endif
		}

	}


}

void BattleManager::onEntityEffectEvent( CCObject* e )
{
	GameEntityEffectEvt* evt = dynamic_cast<GameEntityEffectEvt*>(e);
	if(!evt) return;

	if( evt->name.compare("Death Grip") == 0 ) {
		CCLog("death grip");

		kmVec2 pOrigin;
		kmVec2 pTarget;
		if( !GetEntityPosition(evt->origin, pOrigin) ||  !GetEntityPosition(evt->target, pTarget) )
		{
			CCLog("aborting death grip evt due to invalid target(s)");
		}
		
		kmVec2 toTarget;
		kmVec2Subtract(&toTarget, &pTarget, &pOrigin);

		kmVec2 u_toTarget;
		kmVec2Normalize(&u_toTarget, &toTarget);

		float leashDistance = 1;  //one game unit

		kmVec2 dv;
		kmVec2Scale(&dv, &u_toTarget, leashDistance );

		kmVec2 posEnd;
		kmVec2Add(&posEnd, &pOrigin, &dv);

		//convert back to screen coordinates
		kmVec2Scale(&posEnd, &posEnd, VIEW_UNIT_CONVERSION);

		GameEntityView* tView =	getViewForEntity(evt->target);
		tView->setPosition(posEnd.x, posEnd.y);
	}

}

void BattleManager::onEntityDeath( CCObject* e )
{
	GameEntityDeathEvt* evt = dynamic_cast<GameEntityDeathEvt*>(e);
	if(!evt) return;

	bool killerIsPlayer = false;
	for( int i=0; i< m_players.size(); i++)
	{
		if( m_players[i].model == evt->killer ) 
		{
			killerIsPlayer = true;
			break;
		}
	}

	if( killerIsPlayer ) {
		CCLOG("reward XP");

		float rawXP = evt->killed->getProperty("xp_curr");
		int numPlayers = m_players.size();

		int xpPer = ceilf( rawXP / numPlayers );

		for( int i=0; i< m_players.size(); i++)
		{
			m_players[i].model->incProperty("xp_curr", xpPer, NULL);
		}

	}

	

}

void BattleManager::onEntityLevelup( CCObject* e )
{
	GameEntityLevelupEvt* evt = dynamic_cast<GameEntityLevelupEvt*>(e);
	if(!evt) return;

	GameEntity* entity = dynamic_cast<GameEntity*>( evt->target );

	if( !CastWorldModel::get()->isValid( entity )  ) return;

	//award full health/mana
	entity->setProperty("hp_curr", entity->getProperty("hp_base"), NULL );
	entity->setProperty("mana_curr", entity->getProperty("mana_base"), NULL );
	
}

void BattleManager::setCardDeath( GameEntityView* view )
{
	CCSequence* seq = CCSequence::create(
		CCScaleTo::create(1.f, 1.2f, 0.1f),
		CCCallFunc::create(view, callfunc_selector( GameEntityView::removeFromParent ) ),
		NULL
		);

	view->runAction( seq );
}

void BattleManager::enemyMovementAI( int enemyIdx, float dt )
{
	float speed = 75.0f; //5 pixels/sec
	EntityPair& enemy = m_enemies[enemyIdx];
	
	std::vector<kmVec2> impulses; //x, y
	std::vector<float> impulseWeights;

	//TODO: select from closest player
	EntityPair& player =  m_players[0];
	
	CCSize eSize = enemy.view->getContentSize();
	CCPoint ePos = enemy.view->getPosition();
	ePos.x += eSize.width/2;
	ePos.y += eSize.height/2; //convert to center origin
	CCSize pSize = player.view->getContentSize();
	CCPoint pPos = player.view->getPosition();
	pPos.x += pSize.width/2;
	pPos.y += pSize.height/2; //convert to center origin
	
	float playerLeash = pSize.width * 1.1f;
	float playerLeashSq = playerLeash*playerLeash;

	//impulse towards the player
	kmVec2 toPlayer = { pPos.x - ePos.x, pPos.y - ePos.y };
	kmVec2 u_toPlayer;
	kmVec2Normalize( &u_toPlayer, &toPlayer);
	
	
	if( kmVec2LengthSq( &toPlayer ) < (playerLeashSq * 0.95f) )
	{
		//impulse away from player (too close)
		kmVec2 u_fromPlayer;
		kmVec2Scale(&u_fromPlayer, &u_toPlayer, -1*0.75f); //flip the 'to player' vector
		
		impulses.push_back(u_fromPlayer);
		impulseWeights.push_back(100); //vastly overweigh the impulse to the player
	}else {

		if( kmVec2LengthSq(&toPlayer) > playerLeashSq )
		{
			//kmVec2Scale(&u_toPlayer, &u_toPlayer, 0.51f);
			//impulse towards player
			impulses.push_back(u_toPlayer);
			impulseWeights.push_back(100);

		}
	}
	
	//add impulses away from other enemies
	for( int i=0; i< m_enemies.size(); i++) {
		if( i == enemyIdx ) continue;
		
		CCSize nSize = m_enemies[i].view->getContentSize();
		CCPoint nPos = m_enemies[i].view->getPosition();
		nPos.x += nSize.width/2;
		nPos.y += nSize.height/2;
		
		kmVec2 toNeighbor = { nPos.x - ePos.x, nPos.y - ePos.y };

		float neighborLeash = nSize.width;
		if( kmVec2LengthSq(&toNeighbor) < neighborLeash * neighborLeash ) {
			kmVec2 u_toNeighbor;
			kmVec2Normalize(&u_toNeighbor, &toNeighbor);
			kmVec2 u_fromNeighbor;
			kmVec2Scale(&u_fromNeighbor, &u_toNeighbor, -1);
			impulses.push_back(u_fromNeighbor);
			impulseWeights.push_back(50);
		}
	}
	
	if( impulses.size() == 0 ) return;

	//blend impulses
	kmVec2 finalImpulse = impulses[0]; //zero always valid because its the impulse to the player
	float finalImpulseWeight = impulseWeights[0];
	for( int i=1; i< impulses.size(); i++) {

		float w1 = finalImpulseWeight;
		float w2 = impulseWeights[i];
		float wTot =  w1 + w2;
		
		kmVec2 blend;
		blend.x = (finalImpulse.x * w1 / wTot) + (impulses[i].x * w2 / wTot);
		blend.y = (finalImpulse.y * w1 / wTot) + (impulses[i].y * w2 / wTot);
		
		//run-length summation
		finalImpulseWeight = wTot;
		finalImpulse = blend;
	}
	
	kmVec2 scaledImpulse;
	kmVec2Scale(&scaledImpulse, &finalImpulse, speed * dt);
	
	//CCLog("impulse %.4f", kmVec2Length(& finalImpulse));
	if( kmVec2Length(& finalImpulse) <  (0.5f) )
	{
		CCLog("ignore impulse %.4f", kmVec2Length(& finalImpulse));
		return; //ignore very small changes to avoid leash jitter
	}
	
	ePos.x += scaledImpulse.x;
	ePos.y += scaledImpulse.y;
	ePos.x -= eSize.width/2;
	ePos.y -= eSize.height/2; //back to original anchor coords
	enemy.view->setPosition(ePos);
}

void BattleManager::initAbilities()
{
	CastCommandModel* mod = NULL;

	{
		Json::Value attack;

		attack["name"] = "sword attack";
		attack["castTime"] = 0.15f; //seconds
		attack["cooldownTime"] = 1.85f; //seconds
		attack["range"] = 1.0f; //melee range

		Json::Value swordEffect; //direct damage
		swordEffect["effectType"] = "damage";
		swordEffect["damageType"] = "piercing";
		swordEffect["targetStat"] = "hp_curr";
		swordEffect["valueBase"] = 2.0f;
		swordEffect["valueStat"] = "str"; //note: intellect
		swordEffect["valueMultiplier"] = 2.0f;
		swordEffect["react"] = "shake";
		attack["effectsOnCast"].append( swordEffect );

		mod = new CastCommandModel( attack );
		mod->retain();
		m_abilities[mod->getName()] = mod;
	}

	{
		Json::Value grip;

		grip["name"] = "Death Grip";
		grip["castTime"] = 0.0f; //seconds
		grip["cooldownTime"] = 3.0f; //seconds
		grip["range"] = 3.0f; //melee range

		Json::Value effect1; //debuff
		effect1["effectType"] = "event";
		effect1["valueBase"] = 2.0f;
		effect1["react"] = "debuff";
		grip["effectsOnCast"].append( effect1 );

		mod = new CastCommandModel( grip );
		mod->retain();
		m_abilities[mod->getName()] = mod;
	}

	{
		Json::Value bite;
		bite["name"] = "Bite";
		bite["castTime"] = 0.10f; //seconds
		bite["cooldownTime"] = 1.75f; //seconds
		bite["range"] = 1.0f; //melee range

		Json::Value effect1; //debuff
		effect1["effectType"] = "debuff";
		effect1["targetStat"] = "str_curr";
		effect1["effectLifetime"] = 3.0f; //seconds
		effect1["valueBase"] = 2.0f;
		effect1["react"] = "debuff";
		bite["effectsOnCast"].append( effect1 );

		Json::Value swordEffect; //direct damage
		swordEffect["effectType"] = "damage";
		swordEffect["damageType"] = "piercing";
		swordEffect["targetStat"] = "hp_curr";
		swordEffect["valueBase"] = 2.0f;
		swordEffect["react"] = "shake";
		bite["effectsOnCast"].append( swordEffect );

		mod = new CastCommandModel( bite );
		mod->retain();
		m_abilities[mod->getName()] = mod;
	}

	{
		Json::Value curse;
		curse["name"] = "Curse of Weakness";
		curse["castTime"] = 0.0f; //seconds
		curse["cooldownTime"] = 0.15f; //seconds
		curse["range"] = 3.0f; //melee range

		Json::Value effect1; //direct damage
		effect1["effectType"] = "debuff";
		effect1["targetStat"] = "str_curr";
		effect1["effectLifetime"] = 7.0f; //seconds
		effect1["valueBase"] = 1.0f;
		effect1["react"] = "debuff";
		curse["effectsOnCast"].append( effect1 );

		mod = new CastCommandModel( curse );
		mod->retain();
		m_abilities[mod->getName()] = mod;
	}

	{
		Json::Value spell;

		spell["name"] = "fireball";
		spell["castTime"] = 1.5f;
		spell["travelSpeed"] = 5.0f; //five units per second
		spell["range"] = 5;
		spell["cooldownTime"] = 0.15f;
		spell["effectWhileTravel"] = true;
		spell["stopOnHit"] = true;
		spell["costStat"] = "mana_curr";
		spell["costVal"] = 10; //10 mana
	
		Json::Value spellEffect1; //direct damage
		spellEffect1["effectType"] = "damage";
		spellEffect1["damageType"] = "fire";
		spellEffect1["targetStat"] = "hp_curr";
		spellEffect1["valueBase"] = 2.0f;
		spellEffect1["valueStat"] = "int"; //note: intellect
		spellEffect1["valueMultiplier"] = 1.0f;
		spellEffect1["react"] = "shake";
		spell["effectsOnCast"].append( spellEffect1 );

		Json::Value spellEffect2; //dot
		spellEffect2["effectType"] = "damage";
		spellEffect2["damageType"] = "fire";
		spellEffect2["targetStat"] = "hp_curr";
		spellEffect2["valueBase"] = 1.0f;
		spellEffect2["valueStat"] = "int";
		spellEffect2["valueMultiplier"] = 0.1f;
		spellEffect2["aoeRadius"] = 1.0f;

		spellEffect2["tickFreq"] = 0.5f; //every half second
		spellEffect2["effectLifetime"] = 3.5f;  //so 7 ticks
		spellEffect2["react"] = "burn";
		spellEffect2["stackFlag"] = "burn";
		spell["effectsOnCast"].append( spellEffect2 );

		mod = new CastCommandModel( spell );
		mod->retain();
		m_abilities[mod->getName()] = mod;	

	}




	{
		Json::Value heal;

		heal["name"] = "Heal";
		heal["castTime"] = 0.75f; //seconds
		heal["cooldownTime"] = 0.25f; //seconds
		heal["range"] = 2.0f; //spear range

		Json::Value effect1; //direct heal
		effect1["effectType"] = "heal";
		effect1["targetStat"] = "hp_curr";
		effect1["valueBase"] = 30.0f;
		effect1["valueStat"] = "int"; //note: intellect
		effect1["valueMultiplier"] = 2.0f;
		effect1["react"] = "heal";
		heal["effectsOnCast"].append( effect1 );

		mod = new CastCommandModel( heal );
		mod->retain();
		m_abilities[mod->getName()] = mod;
	}

	{
		Json::Value lifedrain;

		lifedrain["name"] = "Life Drain";
		lifedrain["castTime"] = 0.0f; //seconds
		lifedrain["channelTime"] = 0.90f; //seconds
		lifedrain["channelFreq"] = 0.90f / 4; //seconds
		lifedrain["cooldownTime"] = 0.10f; //seconds
		lifedrain["range"] = 1.5f; //self target range

		Json::Value returnEffect;
		returnEffect["effectType"] = "heal";
		returnEffect["targetStat"] = "hp_curr";
		returnEffect["react"] = "heal";

		Json::Value effect1; //direct damage
		effect1["effectType"] = "damage";
		effect1["targetStat"] = "hp_curr";
		effect1["valueBase"] = 2.0f;
		effect1["valueStat"] = "int"; //note: intellect
		effect1["valueMultiplier"] = 2.0f;
		effect1["react"] = "lifedrain";
		effect1["returnEffect"] = returnEffect;

		effect1["travelSpeed"] = 0.0f;
		lifedrain["effectsOnChannel"].append( effect1 );

		mod = new CastCommandModel( lifedrain );
		mod->retain();
		m_abilities[mod->getName()] = mod;
	}



}

bool BattleManager::GetVecBetween( ICastEntity* from, ICastEntity* to, kmVec2& distVec )
{
	CastWorldModel* world = CastWorldModel::get();

	if( !world->isValid(from) || !world->isValid(to) ) return false;

	kmVec2 pFrom;
	GameEntityView* fromView = getViewForEntity(from);
	if( fromView != NULL ) 
	{
		pFrom.x = fromView->getPositionX();
		pFrom.y = fromView->getPositionY();
	}
	
	kmVec2 pTo;
	GameEntityView* toView = getViewForEntity(to);
	if( toView != NULL ) 
	{
		pTo.x = toView->getPositionX();
		pTo.y = toView->getPositionY();
	}

	kmVec2Subtract( &distVec, &pFrom, &pTo );
	kmVec2Scale(&distVec, &distVec, GAME_UNIT_CONVERSION ); //safe to operate on same vector

	return true;
}

GameEntityView* BattleManager::getViewForEntity( ICastEntity* entity )
{

	for( int i=0; i< m_allEntities.size() ; i++ )
	{
		if( entity == m_allEntities[i].model )
		{
			return m_allEntities[i].view;
		}
	}

	return NULL;
}

bool BattleManager::GetEntityPosition( ICastEntity* entity, kmVec2& pos )
{
	GameEntityView* view = getViewForEntity( entity );

	if( view != NULL ) {
		pos.x = view->getPositionX();
		pos.y = view->getPositionY();
		kmVec2Scale(&pos, &pos, GAME_UNIT_CONVERSION);
		return true;
	}

	return false;
}

bool BattleManager::GetEntitiesInRadius( kmVec2 p, float r, std::vector<ICastEntity*>& entities )
{

	bool found = false;

	auto upscale = VIEW_UNIT_CONVERSION;
	kmVec2Scale( &p, &p, upscale );
	r *= VIEW_UNIT_CONVERSION; //convert to pixels

	float rSq = r*r;
	for( int i=0; i< m_allEntities.size(); i++)
	{
		kmVec2 ePos;
		ePos.x = m_allEntities[i].view->getPositionX();
		ePos.y = m_allEntities[i].view->getPositionY();


		kmVec2 dist;
		kmVec2Subtract( &dist, &p, &ePos );
		//kmVec2Scale( &dist, &dist, GAME_UNIT_CONVERSION ); //safe to operate on same vector

		CCLog("ent %d in radius dist %f", i, kmVec2Length(&dist));

		if( kmVec2LengthSq(&dist) <= rSq ) {
			entities.push_back( m_allEntities[i].model );
			found = true;
		}
	}

	return found;
}


void BattleManager::spawnEnemy()
{
	CCSize screen = boundingBox().size;

	EntityPair enemy;
	enemy.model =  new GameEntity("Giant Rat");
	enemy.model->addAbility( m_abilities["Bite"] );
	//enemy.enemyModel->incProperty("hp_curr", -90);
	enemy.model->setProperty("hp_base", 50, NULL);
	enemy.model->setProperty("xp_curr", 10, NULL);
	enemy.view =  new GameEntityView( enemy.model );
	enemy.view->setBackground("rat.png");
	
	int offY = (rand()%100) - 50;
	enemy.view->setPosition( screen.width, 220 + offY	);
	addChild(enemy.view);
	m_enemies.push_back(enemy);

	m_allEntities.push_back(enemy);
}

