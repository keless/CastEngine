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

	float menuY = origin.y + visibleSize.height/2;
	float menuX = origin.x + visibleSize.width/2;

	CCMenuItemFont* menuTravel = CCMenuItemFont::create("Travel", this, menu_selector(MainMenuScreen::menuStartCallback));
	menuTravel->setPosition(ccp( menuX, menuY));


	menuY += menuTravel->getContentSize().height;

	CCMenuItemFont* menuParty = CCMenuItemFont::create("Party Editor", this, menu_selector(MainMenuScreen::menuPartyCallback));
	menuParty->setPosition(ccp( menuX, menuY ));

	menuY += menuTravel->getContentSize().height;

	CCMenuItemFont* menuSpellbook = CCMenuItemFont::create("Spell Editor", this, menu_selector(MainMenuScreen::menuBookCallback));
	menuSpellbook->setPosition(ccp( menuX, menuY ));



	CCMenu* pMenu = CCMenu::create(menuTravel, menuParty, menuSpellbook, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);


	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

#include "ZZEventBus.h"
using namespace ZZ;
void MainMenuScreen::menuStartCallback(CCObject* pSender)
{
	BaseEvent* evt = new BaseEvent("battle");
	EventBus::get("state")->dispatch("switchTo", evt);

}
void MainMenuScreen::menuPartyCallback(CCObject* pSender)
{
	BaseEvent* evt = new BaseEvent("partyEditor");
	EventBus::get("state")->dispatch("switchTo", evt);

}
void MainMenuScreen::menuBookCallback(CCObject* pSender)
{
	BaseEvent* evt = new BaseEvent("spellEditor");
	EventBus::get("state")->dispatch("switchTo", evt);

}