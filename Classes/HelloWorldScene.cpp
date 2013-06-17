#include "HelloWorldScene.h"

USING_NS_CC;

#include "CastWorldModel.h"

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(pSprite, 0);

	initAbilities();

	/*
	CastCommandState* state = new CastCommandState(  m_abilities["fireball"] );
	GameAbilityView* test = new GameAbilityView( state );
	test->setPosition(50, 100);
	addChild(test);
	*/

	CastWorldModel::get()->setPhysicsInterface(this);

	EntityPair player;

	player.model = new GameEntity("Leeroy");
	//m_playerModel->incProperty("hp_curr", -90);
	player.model->incProperty("xp_next", 100);
	player.model->addAbility( m_abilities["fireball"] );
	//player.model->addAbility( m_abilities["sword attack"] );
	//player.model->addAbility( m_abilities["Heal"] );
	player.model->addAbility( m_abilities["Life Drain"] );
	player.model->addAbility( m_abilities["Curse of Weakness"] );

	player.view = new GameEntityView( player.model );
	player.view->setPositionX(50);
	player.view->setPositionY( 350 );
	addChild(player.view);

	m_players.push_back(player);


	player.model = new GameEntity("Derpsan");
	//m_playerModel->incProperty("hp_curr", -90);
	player.model->incProperty("xp_next", 100);
	//player.model->addAbility( m_abilities["fireball"] );
	player.model->addAbility( m_abilities["sword attack"] );
	//player.model->addAbility( m_abilities["Heal"] );
	//player.model->addAbility( m_abilities["Life Drain"] );
	//player.model->addAbility( m_abilities["Curse of Weakness"] );

	player.view = new GameEntityView( player.model );
	player.view->setPositionX(50);
	player.view->setPositionY( 150 );
	addChild(player.view);

	m_players.push_back(player);

	spawnEnemy();


	//m_playerModel->getTarget()->addTargetEntity(m_enemyModel);
	//m_enemyView->setHighlighted(true);

	//m_enemyModel->getTarget()->addTargetEntity(m_playerModel);
	
	scheduleUpdate();
	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

void HelloWorld::spawnEnemy()
{
	CCLog("spawn enemy");
	CCSize screen = boundingBox().size;

	EntityPair enemy;
	enemy.model =  new GameEntity("Giant Rat");
	enemy.model->addAbility( m_abilities["Bite"] );
	//enemy.enemyModel->incProperty("hp_curr", -90);
	enemy.model->incProperty("hp_base", -50);
	enemy.view =  new GameEntityView( enemy.model );
	enemy.view->setBackground("rat.png");
	
	int offY = (rand()%100) - 50;
	enemy.view->setPosition( screen.width, 220 + offY	);
	addChild(enemy.view);
	m_enemies.push_back(enemy);
}


#define MAX_STEP 1.0f
#define TIME_SCALE 0.5f
#include "CastCommandTime.h"
#include "CastCommandScheduler.h"
#include "CastWorldModel.h"
//virtual 
void HelloWorld::update( float dt )
{
	if( dt > MAX_STEP ) dt = MAX_STEP;

	dt *= TIME_SCALE;

	CastCommandTime::updateDelta(dt);
	CastCommandScheduler::get()->update(dt);
	CastWorldModel::get()->updateStep(dt);

	for( int i=m_enemies.size()-1; i >= 0; i--) {
		EntityPair& enemy = m_enemies[i];

		if( enemy.model->getProperty("hp_curr") <= 0 ) {
			setCardDeath(enemy.view);
			//removeChild(enemy.enemyView);
			CC_SAFE_RELEASE_NULL(enemy.model);
			CC_SAFE_RELEASE_NULL(enemy.view);

			m_enemies.erase( m_enemies.begin() + i );
		}
		else {
			enemyMovementAI(i, dt);

			PerformEnemyAI(enemy.model);
		}
	}

	if( m_enemies.size()  == 0 ) {
		int randNum = rand() % 3;
		CCLog("spawn %d enemies", randNum);
		for( int i=0; i< randNum; i++) {
			CCLog("spawn");
			spawnEnemy();
		}
	}
	
	
	for( int i=0; i< m_players.size(); i++)
	{
		PerformPlayerAi(m_players[i].model);
	}

	
}

//#define DISABLE_ATTACKS

void HelloWorld::PerformEnemyAI( GameEntity* enemy )
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

void HelloWorld::PerformPlayerAi( GameEntity* player )
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
			target = m_enemies[0].model;
			player->getTarget()->addTargetEntity(target);
		}
	}

	if( target != NULL && player->canCast() ) {

#ifndef DISABLE_ATTACKS
		cast->startCast();
#endif

	}


}

void HelloWorld::setCardDeath( GameEntityView* view )
{
	CCSequence* seq = CCSequence::create(
		CCScaleTo::create(1.f, 1.2f, 0.1f),
		CCCallFunc::create(view, callfunc_selector( GameEntityView::removeFromParent ) ),
		NULL
		);

	view->runAction( seq );
}

void HelloWorld::enemyMovementAI( int enemyIdx, float dt )
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
	
	float playerLeash = pSize.width * 1.5f;
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

void HelloWorld::initAbilities()
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
		lifedrain["range"] = 0.0f; //self target range

		Json::Value returnEffect;
		returnEffect["effectType"] = "heal";
		returnEffect["targetStat"] = "hp_curr";
		returnEffect["react"] = "heal";

		Json::Value effect1; //direct heal
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

#define GAME_UNIT_CONVERSION (1.0f/210.0f)

bool HelloWorld::GetVecBetween( ICastEntity* from, ICastEntity* to, kmVec2& distVec )
{
	CastWorldModel* world = CastWorldModel::get();

	if( !world->isValid(from) || !world->isValid(to) ) return false;

	kmVec2 pFrom;
	bool found = false;
	for( int i=0; i< m_enemies.size()  && !found; i++ )
	{
		if( from == m_enemies[i].model )
		{
			pFrom.x = m_enemies[i].view->getPositionX();
			pFrom.y = m_enemies[i].view->getPositionY();
			found = true;
			break;
		}
	}
	for( int i=0; i< m_players.size() && !found; i++) {
		if( from == m_players[i].model )
		{
			pFrom.x = m_players[i].view->getPositionX();
			pFrom.y = m_players[i].view->getPositionY();
			found = true;
			break;
		}
	}
	

	kmVec2 pTo;
	found = false;
	for( int i=0; i< m_enemies.size() && !found; i++ )
	{
		if( from == m_enemies[i].model )
		{
			pTo.x = m_enemies[i].view->getPositionX();
			pTo.y = m_enemies[i].view->getPositionY();
			found = true;
			break;
		}
	}
	for( int i=0; i< m_players.size() && !found; i++ )
	{
		if( from == m_players[i].model )
		{
			pTo.x = m_players[i].view->getPositionX();
			pTo.y = m_players[i].view->getPositionY();
			found = true;
			break;
		}
	}


	kmVec2Subtract( &distVec, &pFrom, &pTo );
	kmVec2Scale(&distVec, &distVec, GAME_UNIT_CONVERSION ); //safe to operate on same vector

	return true;
}

bool HelloWorld::GetEntityPosition( ICastEntity* entity, kmVec2& pos )
{
	bool found = false;
	for( int i=0; i< m_enemies.size() && !found; i++ )
	{
		if( entity == m_enemies[i].model )
		{
			pos.x = m_enemies[i].view->getPositionX();
			pos.y = m_enemies[i].view->getPositionY();
			found = true;
			break;
		}
	}
	for( int i=0; i< m_players.size() && !found; i++ )
	{
		if( entity == m_players[i].model )
		{
			pos.x = m_players[i].view->getPositionX();
			pos.y = m_players[i].view->getPositionY();
			found = true;
			break;
		}
	}

	if( found ) {
		kmVec2Scale(&pos, &pos, GAME_UNIT_CONVERSION);
	}

	return found;
}

bool HelloWorld::GetEntitiesInRadius( kmVec2 p, float r, std::vector<ICastEntity*>& entities )
{

	bool found = false;

	kmVec2Scale( &p, &p, 1.0f/GAME_UNIT_CONVERSION );
	r /= GAME_UNIT_CONVERSION; //convert to pixels

	float rSq = r*r;
	for( int i=0; i< m_enemies.size(); i++)
	{
		kmVec2 ePos;
		ePos.x = m_enemies[i].view->getPositionX();
		ePos.y = m_enemies[i].view->getPositionY();


		kmVec2 dist;
		kmVec2Subtract( &dist, &p, &ePos );
		//kmVec2Scale( &dist, &dist, GAME_UNIT_CONVERSION ); //safe to operate on same vector

		CCLog("dist %f", kmVec2LengthSq(&dist));

		if( kmVec2LengthSq(&dist) <= rSq ) {
			entities.push_back( m_enemies[i].model );
			found = true;
		}
	}

	//check player
	for( int i=0; i< m_players.size(); i++)
	{
		kmVec2 ePos;
		ePos.x = m_players[i].view->getPositionX();
		ePos.y = m_players[i].view->getPositionY();


		kmVec2 dist;
		kmVec2Subtract( &dist, &p, &ePos );
		//kmVec2Scale( &dist, &dist, GAME_UNIT_CONVERSION ); //safe to operate on same vector

		if( kmVec2LengthSq(&dist) <= rSq ) {
			entities.push_back( m_players[i].model );
			found = true;
		}
	}

	return found;
}

inline CCPoint locationInGLFromTouch(CCTouch& touch)
{
   auto director = CCDirector::sharedDirector();
   return director->convertToGL(touch.getLocationInView());
}


void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		//m_playerView->ccTouchBegan(touch, event);
	}
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		//m_playerView->ccTouchMoved(touch, event);
	}
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;
		/*
		m_playerView->ccTouchEnded(touch, event);
		if( m_playerModel->canCast() ) {
			std::vector<GameAbilityView*>& abilityViews = m_playerView->getAbilityViews();
			for( int i=0; i< abilityViews.size(); i++)
			{
				abilityViews[i]->ccTouchEnded(touch, event);
			}
		}

		int touchedIdx = -1;
		for( int i=0; i< m_enemies.size(); i++) 
		{
			if( m_enemies[i].enemyView->boundingBox().containsPoint( touch->getLocation() ) )
			{
				touchedIdx = i;
				break;
			}
		}

		if( touchedIdx != -1 ) {
			//dehighlight previous targets
			CastTarget* pTarget = m_playerModel->getTarget();
			for( int tIdx =0 ; tIdx< pTarget->getEntityList().size(); tIdx++) 
			{
				GameEntity* entity = (GameEntity*)pTarget->getEntityList()[tIdx];

				for( int vIdx=0; vIdx < m_enemies.size() ; vIdx++)
				{
					if( entity == m_enemies[vIdx].enemyModel )
					{
						GameEntityView* view = m_enemies[vIdx].enemyView;
						view->setHighlighted(false); 
						break;
					}
				}

			}
			pTarget->clearTargetEntities();
				
		
			//highlight new target
				
			pTarget->addTargetEntity( m_enemies[touchedIdx].enemyModel);
			m_enemies[touchedIdx].enemyView->setHighlighted(true);
			
		}
		*/

	}
}
void HelloWorld::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		//m_playerView->ccTouchCancelled(touch, event);
	}
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
