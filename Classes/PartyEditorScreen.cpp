#include "PartyEditorScreen.h"


PartyEditorScreen::PartyEditorScreen(void)
{
	m_partyList = NULL;
	m_partyMemberEditor = NULL;

	EventBus::get("partyMemberSelect")->addListener("zzrgrTrigger", this, callfuncO_selector(PartyEditorScreen::onPartyMemberSelected));
}


PartyEditorScreen::~PartyEditorScreen(void)
{
	EventBus::get("partyMemberSelect")->remListener("zzrgrTrigger", this, callfuncO_selector(PartyEditorScreen::onPartyMemberSelected));
}

bool PartyEditorScreen::init()
{
	if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();


	m_partyList = PartyList::create();
	m_partyList->setAnchorPoint(ccp(0,0));
	m_partyList->setPosition(0,0);
	addChild(m_partyList);

	return true;
}


void PartyEditorScreen::onPartyMemberSelected(CCObject* e)
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(evt == NULL) return;

	int partyMemberIdx = evt->json.get("index", -1).asInt();
	CCLog("party member %d selected", partyMemberIdx);

	if( m_partyMemberEditor != NULL ) 
	{
		m_partyMemberEditor->removeAllChildrenWithCleanup(true);
	}

	int pListW = m_partyList->getContentSize().width;

	CCSize size = getContentSize();
	GameEntity* entity = m_partyList->getEntity(partyMemberIdx);

	size.width -= pListW;

	m_partyMemberEditor = PartyMemberEditor::create(entity, size);
	m_partyMemberEditor->setAnchorPoint(ccp(0,0));
	m_partyMemberEditor->setPositionX(pListW);
	addChild(m_partyMemberEditor);
}