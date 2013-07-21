#include "PartyList.h"


PartyList::PartyList(void)
{
	
	EventBus::game()->addListener("addPartyMember", this, callfuncO_selector(PartyList::onAddPartyMember));
}


PartyList::~PartyList(void)
{
	EventBus::game()->remListener("addPartyMember", this, callfuncO_selector(PartyList::onAddPartyMember));
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
		radio->setPosition( m_cellWidth/2, m_cellHeight/2 + i*m_cellHeight);
		radio->setContentSize(CCSizeMake(m_cellWidth, m_cellHeight));

		addChild(radio);
		m_partyButtons[i] = radio;

		radio->setVisible(false); //all off until filled

	}

	for( int i=0; i< MAX_PARTY_MEMBERS; i++) {

		TouchableNode* btn = CreateSimpleButton("+ Party Member", "addPartyMember");
		btn->setAnchorPoint(ccp(0.5,0.5));
		btn->setPosition( m_cellWidth/2, m_cellHeight/2 + i*m_cellHeight);

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

}

void PartyList::loadEntitiesForPartyJson()
{
	//TODO: load party JSON
	m_partyJson = ReadFileToJson("party.json");
}

void PartyList::onAddPartyMember(CCObject* e)
{
	if( m_entities.size() >= MAX_PARTY_MEMBERS ) return;
	int numEntitiesStart = m_entities.size();

	GameEntity* ge = new GameEntity("foo");
	m_entities.push_back(ge);

	GameEntityView* view = new GameEntityView( m_entities[m_entities.size()-1] );
	view->setAnchorPoint(ccp(0.5,0.5));
	view->setPosition( m_cellWidth/2, m_cellHeight/2);

	m_partyButtons[ numEntitiesStart ]->addChild(view);
	m_partyButtons[ numEntitiesStart ]->setVisible(true);
	
	m_partyAddButtons[ numEntitiesStart ]->setVisible(false);
	
	
	//todo: highlight new cell

}


/*

//virtual 
CCTableViewCell* PartyList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell *cell = table->dequeueCell();  

	if(cell == NULL ) {

		cell = new CCTableViewCell();
		//cell->autorelease();

		cell->setAnchorPoint(ccp(0,0));


	}else {
		cell->removeAllChildren();
	}

	if( idx == m_entities.size() ) {
		//create + player button
		CCNode* btn = CreateSimpleButton("+ Party Member", "addPartyMember");
		btn->setAnchorPoint(ccp(0.5,0.5));
		btn->setPosition( m_cellWidth/2, m_cellHeight/2);
		cell->addChild(btn);
	}else {
		//load character

		GameEntityView* view = new GameEntityView( m_entities[idx] );
		view->setAnchorPoint(ccp(0.5,0.5));
		view->setPosition( m_cellWidth/2, m_cellHeight/2);
		
		BaseRadioGroupLayer* radio = new BaseRadioGroupLayer();
		radio->initRadioGroup("partyMemberSelect", idx);
		radio->addChild(view);
		radio->setAnchorPoint(ccp(0.5,0.5));
		radio->setPosition( m_cellWidth/2, m_cellHeight/2);
		radio->setContentSize(CCSizeMake(m_cellWidth, m_cellHeight));

		cell->addChild(radio);

		view->release();
	}

	return cell;
}

*/