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

	m_cellWidth = 200 + 10;
	m_cellHeight = 200 + 10;

	int w = fmax(visibleSize.width/5, m_cellWidth);
	int h = visibleSize.height;

	setContentSize(CCSizeMake(w, h));
	ignoreAnchorPointForPosition(false);

	m_bg = CCLayerColor::create(ccc4(240,227,132,125), w, h);
	addChild(m_bg);

	//TODO: load party JSON
	m_partyJson = ReadFileToJson("party.json");



	m_list = CCTableView::create(this, CCSizeMake(w, h));
	m_list->setDirection(kCCScrollViewDirectionVertical);
    //m_list->setDelegate(this);
    m_list->setVerticalFillOrder(kCCTableViewFillTopDown);
	addChild(m_list);
	
	m_list->reloadData();

	
	return true;
}

void PartyList::loadEntitiesForPartyJson()
{
	//todo
}

void PartyList::onAddPartyMember(CCObject* e)
{
	GameEntity* ge = new GameEntity("foo");
	m_entities.push_back(ge);

	m_list->reloadData();

}

//virtual 
CCSize PartyList::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake( m_cellWidth, m_cellHeight);
}

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
		view->setHighlighted(true);
		cell->addChild(view);

		view->release();
	}

	return cell;
}
//virtual 
unsigned int PartyList::numberOfCellsInTableView(CCTableView *table)
{
	//return m_spellNames.size();
	return m_entities.size() + 1;
}