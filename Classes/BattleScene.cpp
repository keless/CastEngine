#include "BattleScene.h"

USING_NS_CC;



CCScene* BattleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    BattleScene *layer = BattleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BattleScene::init()
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
                                        menu_selector(BattleScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    CCSprite* pSprite = CCSprite::create("workbench2.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pSprite->setContentSize(visibleSize);
    this->addChild(pSprite, 0);

	//todo: remove listener when this state goes out of scope (doesnt happen?)
	EventBus::get("state")->addListener("switchTo", this, callfuncO_selector(BattleScene::doStateChange));

	doStateChange( new BaseEvent("mainMenu") );

	setTouchEnabled(true);
	//registerWithTouchDispatcher();

	JsonManager::get()->initFromFile("spreadsheet.json");
    
    return true;
}

//virtual 
void BattleScene::update( float dt )
{

	
}



inline CCPoint locationInGLFromTouch(CCTouch& touch)
{
   CCDirector* director = CCDirector::sharedDirector();
   return director->convertToGL(touch.getLocationInView());
}


void BattleScene::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	for(CCSetIterator it = touches->begin(); it != touches->end(); it++) 
	{
		CCTouch* touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;
	}
}

void BattleScene::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	for(CCSetIterator it = touches->begin(); it != touches->end(); it++) 
	{
		CCTouch* touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;
	}
}

void BattleScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	for(CCSetIterator it = touches->begin(); it != touches->end(); it++) 
	{
		CCTouch* touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;
	}
}
void BattleScene::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
	for(CCSetIterator it = touches->begin(); it != touches->end(); it++) 
	{
		CCTouch* touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;
	}
}

#include "BattleManagerScreen.h"
#include "MainMenuScreen.h"
#include "SpellEditScreen.h"
#include "PartyEditorScreen.h"
void BattleScene::doStateChange( CCObject* e )
{
	CCLog("start state change");

	BaseEvent* evt = dynamic_cast<BaseEvent*>(e);
	if( evt == NULL ) return;


	if( m_activeLayer != NULL ) {
		m_activeLayer->removeFromParentAndCleanup(true);
		//m_activeLayer->autorelease(); //let the actual class die at the end of the current loop
		//CC_SAFE_RELEASE_NULL(m_activeLayer);  //dont need to release here
	}

	std::string state = evt->type;

	if( state.compare("battle") == 0 ) {
		m_activeLayer = BattleManagerScreen::create();
	}else if( state.compare("mainMenu") == 0 ) {
		m_activeLayer = MainMenuScreen::create();
	}else if( state.compare("spellEditor") == 0 ) {
		m_activeLayer = SpellEditScreen::create();
	}else if( state.compare("partyEditor") == 0 ) {
		m_activeLayer = PartyEditorScreen::create();
	}

	if( m_activeLayer != NULL ) 
	{
		addChild(m_activeLayer);
	}

}

void BattleScene::menuCloseCallback(CCObject* pSender)
{

	if( m_activeLayer == NULL || dynamic_cast<MainMenuScreen*>(m_activeLayer) != NULL ) {
		//main menu or no state-- kill app
		CCDirector::sharedDirector()->end();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	}else {
		//move back to main menu
		doStateChange(new BaseEvent("mainMenu"));
	}


}
