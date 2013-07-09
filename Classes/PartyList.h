#ifndef _PARTYLIST_H_
#define _PARTYLIST_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "cocos-ext.h"
using namespace extension;


class PartyList : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
	CCTableView* m_list; 
	CCLayerColor* m_bg;
	float m_cellHeight;
	float m_cellWidth;

public:
	PartyList(void);
	~PartyList(void);

	CREATE_FUNC(PartyList);
	bool init();

	virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

#endif
