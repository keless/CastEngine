#include "HelloWorldScene.h"

USING_NS_CC;



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

	m_playerModel = new GameEntity("Leeroy");
	m_playerModel->incProperty("hp_curr", -90);
	m_playerModel->addAbility( m_abilities["fireball"] );
	m_playerModel->addAbility( m_abilities["sword attack"] );
	m_playerModel->addAbility( m_abilities["Heal"] );
	m_playerModel->addAbility( m_abilities["Life Drain"] );

	m_playerView = new GameEntityView( m_playerModel );
	m_playerView->setPositionX(50);
	m_playerView->setPositionY( 250 );
	addChild(m_playerView);

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
		EntityPair enemy;
	enemy.enemyModel =  new GameEntity("Giant Rat");
	//m_enemyModel->incProperty("hp_curr", -90);
	enemy.enemyView =  new GameEntityView( enemy.enemyModel );
	enemy.enemyView->setBackground("rat.png");
	enemy.enemyView->setPosition( 270, 200 );
	addChild(enemy.enemyView);
	m_enemies.push_back(enemy);
}


#include "CastCommandTime.h"
#include "CastCommandScheduler.h"
#include "CastWorldModel.h"
//virtual 
void HelloWorld::update( float dt )
{
	CastCommandTime::updateDelta(dt);
	CastCommandScheduler::get()->update(dt);
	CastWorldModel::get()->updateStep(dt);

	for( int i=m_enemies.size()-1; i >= 0; i--) {
		EntityPair& enemy = m_enemies[i];

		if( enemy.enemyModel->getProperty("hp_curr") <= 0 ) {
			removeChild(enemy.enemyView);
			CC_SAFE_RELEASE_NULL(enemy.enemyModel);
			CC_SAFE_RELEASE_NULL(enemy.enemyView);

			m_enemies.erase( m_enemies.begin() + i );
		}
	}

	if( m_enemies.size()  == 0 ) {
		spawnEnemy();
	}
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
		Json::Value spell;

		spell["name"] = "fireball";
		spell["castTime"] = 1.5f;
		spell["travelSpeed"] = 5.0f; //five units per second
		spell["range"] = 5;
		spell["cooldownTime"] = 0.15f;
		spell["effectWhileTravel"] = true;
		spell["stopOnHit"] = true;
	
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

		m_playerView->ccTouchBegan(touch, event);
	}
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		m_playerView->ccTouchMoved(touch, event);
	}
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		m_playerView->ccTouchEnded(touch, event);

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


	}
}
void HelloWorld::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		m_playerView->ccTouchCancelled(touch, event);
	}
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
