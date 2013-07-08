#include "PartyEditorScreen.h"


PartyEditorScreen::PartyEditorScreen(void)
{
}


PartyEditorScreen::~PartyEditorScreen(void)
{
}

bool PartyEditorScreen::init()
{
	if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCLabelTTF* title = CCLabelTTF::create("more people will come if we tell them we have punch and pie", "Arial", 24, CCSizeMake(400,100), kCCTextAlignmentCenter);
	title->setAnchorPoint(ccp(0.5,1));
	title->setPosition(ccp(visibleSize.width/2, visibleSize.height - 10));
	addChild(title);
}


