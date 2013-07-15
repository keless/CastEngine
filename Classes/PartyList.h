#ifndef _PARTYLIST_H_
#define _PARTYLIST_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "cocos-ext.h"
using namespace extension;

#include "GameEntityView.h"

class PartyList : public CCLayer, public CCTableViewDataSource//, public CCTableViewDelegate
{
	CCTableView* m_list; 
	CCLayerColor* m_bg;
	float m_cellHeight;
	float m_cellWidth;

	Json::Value m_partyJson;
	std::vector<GameEntity*> m_entities;

	void loadEntitiesForPartyJson();

	void onAddPartyMember(CCObject* e);

public:
	PartyList(void);
	~PartyList(void);

	CREATE_FUNC(PartyList);
	bool init();

	GameEntity* getEntity( int idx );

	virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

#endif
