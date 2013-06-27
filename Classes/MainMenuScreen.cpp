#include "MainMenuScreen.h"


MainMenuScreen::MainMenuScreen(void)
{
}


MainMenuScreen::~MainMenuScreen(void)
{

}

// on "init" you need to initialize your instance
bool MainMenuScreen::init()
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
                                        menu_selector(MainMenuScreen::menuStartCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width/2 - pCloseItem->getContentSize().width/2 ,
                                origin.y + visibleSize.height/2 - pCloseItem->getContentSize().height/2));

    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);


	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

#include "ZZEventBus.h"
void MainMenuScreen::menuStartCallback(CCObject* pSender)
{
	ZZEventBus::BaseEvent* evt = new ZZEventBus::BaseEvent("battle");
	ZZEventBus::get("state")->dispatch("switchTo", evt);

}