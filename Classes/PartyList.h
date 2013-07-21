#ifndef _PARTYLIST_H_
#define _PARTYLIST_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "cocos-ext.h"
using namespace extension;

#include "GameEntityView.h"

#define MAX_PARTY_MEMBERS 3

class PartyList : public CCLayer
{
	CCLayerColor* m_bg;
	float m_cellHeight;
	float m_cellWidth;

	Json::Value m_partyJson;
	std::vector<GameEntity*> m_entities;

	BaseRadioGroupLayer* m_partyButtons[MAX_PARTY_MEMBERS];
	CCNode* m_partyAddButtons[MAX_PARTY_MEMBERS];

	void loadEntitiesForPartyJson();

	void onAddPartyMember(CCObject* e);

public:
	PartyList(void);
	~PartyList(void);

	CREATE_FUNC(PartyList);
	bool init();

	GameEntity* getEntity( int idx );

	void refreshList(); //call this when game entities have changed and list needs to be updated

};

#endif
