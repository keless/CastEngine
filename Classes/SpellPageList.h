#ifndef _SPELLPAGELIST_H_
#define _SPELLPAGELIST_H_


#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

#include "ZZEventBus.h"
using namespace ZZ;

class SpellPageList : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
	CCTableView* m_list; 
	CCLayerColor* m_bg;
	float m_cellHeight;
	float m_cellWidth;

public:
	SpellPageList(void);
	~SpellPageList(void);

	CREATE_FUNC(SpellPageList);
	bool init();


	//inherited from CCTableViewDataSource
	//virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx); //implement this if non-uniform cell sizes
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

	//inherited from CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(CCScrollView* view) {}
	virtual void scrollViewDidZoom(CCScrollView* view) {}
};

#endif
