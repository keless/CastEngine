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


	/*
	CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("cs_superlaser.plist");

	//anim is a 'resource' and only one instance is neccesary for multiple animation 'instances' to run
	CCAnimation* anim = CCAnimation::create();
	for( int i=0; i<100; i++) {
		char buff[128];
		snprintf(buff, sizeof(buff), "cs_superlaser_01_%03d.png", i);
		CCSpriteFrame* frame = frameCache->spriteFrameByName(buff);
		if( frame != NULL ) {
			anim->addSpriteFrame(frame);
		}
	}
	anim->setDelayPerUnit(1.0f / 15.0f);
	anim->setLoops(99);

	CCAnimationFrame* firstFrame = (CCAnimationFrame*)anim->getFrames()->objectAtIndex(0);
	CCTexture2D* tex = firstFrame->getSpriteFrame()->getTexture();
	tex->setAliasTexParameters();


	CCSprite* display = CCSprite::create();
	display->runAction( CCAnimate::create(anim));
	display->setPosition(ccp(300,300));
	//display->getTexture()->setAliasTexParameters();
	display->setScale(2);
	this->addChild(display);
	//*/
	
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
