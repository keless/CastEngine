#include "SpellPageList.h"


#define NUM_SPELL_SCRIPTS 14
char* s_spellScriptNames[NUM_SPELL_SCRIPTS];


SpellPageList::SpellPageList(void)
{
	s_spellScriptNames[0] = "01 Novice's Circle";
	s_spellScriptNames[1] = "02 Blind Eye";
	s_spellScriptNames[2] = "03 Adept's Circle";
	s_spellScriptNames[3] = "04 Lesser Pyramid";
	s_spellScriptNames[4] = "05 Serpent's Eye";
	s_spellScriptNames[5] = "06 Lesser Triquetra";
	s_spellScriptNames[6] = "07 Compass";
	s_spellScriptNames[7] = "08 Fortress";
	s_spellScriptNames[8] = "09 Dragon's Eye";
	s_spellScriptNames[9] = "10 Seeing Eye";
	s_spellScriptNames[10] = "11 School";
	s_spellScriptNames[11] = "12 Leaf";
	s_spellScriptNames[12] = "13 Greater Pyramid";
	s_spellScriptNames[13] = "14 Greater Triquetra";

	m_cellWidth = 0;
	m_cellHeight = 0;
}


SpellPageList::~SpellPageList(void)
{
}

bool SpellPageList::init()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	int w = visibleSize.width/5;
	int h = visibleSize.height;

	setContentSize(CCSizeMake(w, h));
	ignoreAnchorPointForPosition(false);

	m_bg = CCLayerColor::create(ccc4(240,227,132,125), w, h);
	addChild(m_bg);

		
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
CCSize SpellPageList::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake( m_cellWidth, m_cellHeight);
}



//virtual 
CCTableViewCell* SpellPageList::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell *cell = table->dequeueCell();  

	if(cell == NULL ) {

		cell = new CCTableViewCell();
		//cell->autorelease();

		cell->setAnchorPoint(ccp(0,0));

		CCLabelTTF* label = CCLabelTTF::create(s_spellScriptNames[idx], "Helvetica", 16.0f);
		label->setColor(ccBLACK);
		label->setAnchorPoint(ccp(0,0));
		label->setPositionY( m_cellHeight/2 );
		label->setTag(1234);
		cell->addChild(label);

	}else {
		CCLabelTTF* label = (CCLabelTTF*)cell->getChildByTag(1234);
		label->setString(s_spellScriptNames[idx]);
	}

	return cell;
}
//virtual 
unsigned int SpellPageList::numberOfCellsInTableView(CCTableView *table)
{
	return NUM_SPELL_SCRIPTS;
}

void SpellPageList::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CCLOG("cell touched at index: %i", cell->getIdx());

	JsonEvent* evt = new JsonEvent("pageSelected");
	evt->json["idx"] = cell->getIdx();
	evt->json["name"] = s_spellScriptNames[cell->getIdx()];

	EventBus::get("spellEdit")->dispatch("pageSelected", evt );
	
}

