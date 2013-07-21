#include "PartyList.h"


PartyList::PartyList(void)
{
	
	EventBus::game()->addListener("addPartyMemberBtn", this, callfuncO_selector(PartyList::onAddPartyMemberBtn));
}


PartyList::~PartyList(void)
{
	EventBus::game()->remListener("addPartyMemberBtn", this, callfuncO_selector(PartyList::onAddPartyMemberBtn));
}

bool PartyList::init()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	ignoreAnchorPointForPosition(false);

	m_cellWidth = 200 + 10;
	m_cellHeight = 200 + 10;

	int w = MAX(visibleSize.width/5, m_cellWidth ); //windows wants to use 'max' and mac wants to use 'maxf'
	int h = visibleSize.height;

	setContentSize(CCSizeMake(w, h));
	ignoreAnchorPointForPosition(false);

	m_bg = CCLayerColor::create(ccc4(240,227,132,125), w, h);
	addChild(m_bg);


	for( int i=0; i< MAX_PARTY_MEMBERS; i++) {

		BaseRadioGroupLayer* radio = new BaseRadioGroupLayer();
		radio->initRadioGroup("partyMemberSelect", i);
		//radio->addChild(view);
		radio->setAnchorPoint(ccp(0.5,0.5));
		radio->setPosition( m_cellWidth/2, h - (m_cellHeight/2 + i*m_cellHeight));
		radio->setContentSize(CCSizeMake(m_cellWidth, m_cellHeight));

		addChild(radio);
		m_partyButtons[i] = radio;

		radio->setVisible(false); //all off until filled

	}

	for( int i=0; i< MAX_PARTY_MEMBERS; i++) {

		TouchableNode* btn = CreateSimpleButton("+ Party Member", "addPartyMemberBtn");
		btn->setAnchorPoint(ccp(0.5,0.5));
		btn->setPosition( m_cellWidth/2, h -( m_cellHeight/2 + i*m_cellHeight));

		addChild(btn);
		m_partyAddButtons[i] = btn;

		btn->setVisible(true);
		btn->setTouchEnabled(true);

	}

	loadEntitiesForPartyJson();

	
	return true;
}

GameEntity* PartyList::getEntity( int idx )
{
	if( idx < 0 || idx >= m_entities.size() ) return NULL; //oob

	return m_entities[idx];
}

void PartyList::refreshList()
{

	//TODO: update game entity views
	for(int i=0; i< m_entities.size(); i++)
	{
		//intentionally using entities size to access party buttons
		GameEntityView* view = dynamic_cast<GameEntityView*>(m_partyButtons[i]->getChildByTag(1234));
		if(!view) continue;

		view->updateView();
	}

	saveEntitiesToPartyJson();

}

void PartyList::loadEntitiesForPartyJson()
{
	//TODO: load party JSON
	Json::Value partyJson = ReadFileToJson("party.json");

	Json::Value::Members partyMembers = partyJson.getMemberNames();
	for( int i=0; i< partyMembers.size() && i < MAX_PARTY_MEMBERS; i++)
	{
		GameEntity* ge = new GameEntity("bar");
		ge->initFromJson( partyJson[partyMembers[i]] );

		addPartyMember(ge);
	}
}

void PartyList::saveEntitiesToPartyJson()
{
	Json::Value json;

	for( int i=0; i< m_entities.size(); i++)
	{
		std::string name = m_entities[i]->getName();
		json[name] = m_entities[i]->toJson();
	}

	WriteJsonToFile( json, "party.json" );
}

void PartyList::addPartyMember( GameEntity* entity )
{
	int numEntitiesStart = m_entities.size();
	m_entities.push_back(entity);

	GameEntityView* view = new GameEntityView( m_entities[m_entities.size()-1] );
	view->setAnchorPoint(ccp(0.5,0.5));
	view->setPosition( m_cellWidth/2, m_cellHeight/2);
	view->setTag(1234);

	m_partyButtons[ numEntitiesStart ]->addChild(view);
	m_partyButtons[ numEntitiesStart ]->setVisible(true);
	
	m_partyAddButtons[ numEntitiesStart ]->setVisible(false);
}


void PartyList::onAddPartyMemberBtn(CCObject* e)
{
	if( m_entities.size() >= MAX_PARTY_MEMBERS ) return;
	
	GameEntity* ge = new GameEntity("foo");
	addPartyMember(ge);

	//todo: save changes to party

	saveEntitiesToPartyJson();
}
