#include "PartyEditorScreen.h"


PartyEditorScreen::PartyEditorScreen(void)
{
	m_partyList = NULL;
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

	m_partyList = PartyList::create();
	m_partyList->setAnchorPoint(ccp(0,0));
	m_partyList->setPosition(0,0);
	addChild(m_partyList);

	return true;
}


