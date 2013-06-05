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
	m_playerModel->retain();
	m_playerModel->addAbility( m_abilities["fireball"] );
	m_playerModel->addAbility( m_abilities["sword attack"] );

	m_playerView = new GameEntityView( m_playerModel );
	m_playerView->setPositionX(50);
	m_playerView->setPositionY( 250 );
	addChild(m_playerView);

	

	m_enemyModel = new GameEntity("Giant Rat");
	m_enemyModel->retain();
	m_enemyView = new GameEntityView( m_enemyModel );
	m_enemyView->setPosition( 230, 200 );
	addChild(m_enemyView );

	m_playerModel->getTarget()->addTargetEntity(m_enemyModel);
	m_enemyModel->getTarget()->addTargetEntity(m_playerModel);
	
	scheduleUpdate();
	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}


#include "CastCommandTime.h"
#include "CastCommandScheduler.h"
//virtual 
void HelloWorld::update( float dt )
{
	CastCommandTime::updateDelta(dt);
	CastCommandScheduler::get()->update(dt);
}

void HelloWorld::initAbilities()
{
	CastCommandModel* mod = NULL;

	Json::Value attack;
	attack["name"] = "sword attack";
	attack["castTime"] = 0.15f; //seconds
	attack["cooldownTime"] = 1.85f; //seconds
	attack["range"] = 1.0f; //melee range

	mod = new CastCommandModel( attack );
	mod->retain();
	m_abilities[mod->getName()] = mod;


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
	spellEffect1["valueBase"] = 2.0f;
	spellEffect1["valueStat"] = "int"; //note: intellect
	spellEffect1["valueMultiplier"] = 1.0f;
	spell["effectsOnCast"].append( spellEffect1 );

	Json::Value spellEffect2; //dot
	spellEffect2["effectType"] = "damage";
	spellEffect1["damageType"] = "fire";
	spellEffect2["valueBase"] = 1.0f;
	spellEffect2["valueStat"] = "int";
	spellEffect2["valueMultiplier"] = 0.1f;
	spellEffect2["effectLifetime"] = 1.0f;
	spellEffect2["stackFlag"] = "burn";
	spell["effectsOnCast"].append( spellEffect2 );



	mod = new CastCommandModel( spell );
	mod->retain();
	m_abilities[mod->getName()] = mod;
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
		if(touch == nullptr)
			break;

		m_playerView->ccTouchBegan(touch, event);
	}
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == nullptr)
			break;

		m_playerView->ccTouchMoved(touch, event);
	}
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == nullptr)
			break;

		m_playerView->ccTouchEnded(touch, event);
	}
}
void HelloWorld::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == nullptr)
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
