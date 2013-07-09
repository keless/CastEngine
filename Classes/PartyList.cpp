#include "PartyList.h"


PartyList::PartyList(void)
{
}


PartyList::~PartyList(void)
{
}

bool PartyList::init()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	int w = visibleSize.width/5;
	int h = visibleSize.height;

	setContentSize(CCSizeMake(w, h));
	ignoreAnchorPointForPosition(false);

	m_bg = CCLayerColor::create(ccc4(240,227,132,125), w, h);
	addChild(m_bg);

	//TODO: load party JSON
		
	m_cellWidth = w;
	m_cellHeight = h/5;

	m_list = CCTableView::create(this, CCSizeMake(w, h));
	m_list->setDirection(kCCScrollViewDirectionVertical);
    m_list->setDelegate(this);
    m_list->setVerticalFillOrder(kCCTableViewFillTopDown);
	addChild(m_list);
	
	m_list->reloadData();

	
	return true;
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

		/* todo; set party view
		CCLabelTTF* label = CCLabelTTF::create(m_spellNames[idx].c_str(), "Helvetica", 16.0f);
		label->setContentSize(CCSizeMake(m_cellWidth, m_cellHeight));
		label->setColor(ccBLACK);
		label->setAnchorPoint(ccp(0,0));
		label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		label->setHorizontalAlignment(kCCTextAlignmentCenter);
		label->setTag(1234);
		cell->addChild(label);
		*/
	


	}else {
		//CCLabelTTF* label = (CCLabelTTF*)cell->getChildByTag(1234);
		//label->setString(m_spellNames[idx].c_str());
	}

	return cell;
}
//virtual 
unsigned int PartyList::numberOfCellsInTableView(CCTableView *table)
{
	//return m_spellNames.size();
	return 0; //todo: look at party list
}