#ifndef _SPELLPAGELIST_H_
#define _SPELLPAGELIST_H_


#include "ZZUtils.h"
using namespace ZZ;

#include "cocos-ext.h"
using namespace extension;



class SpellPageList : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
	CCTableView* m_list; 
	CCLayerColor* m_bg;
	float m_cellHeight;
	float m_cellWidth;

	std::vector<std::string> m_spellNames;

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
