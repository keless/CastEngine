#ifndef _PARTYMEMBEREDITOR_H_
#define _PARTYMEMBEREDITOR_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "GameEntity.h"
#include "GameItemView.h"

class PartyMemberEditor : public CCLayer
{

	GameEntity* m_pEntity;
	GameItem* m_itm[3];

	CCNode* m_editChar;
	CCNode* m_editItems;

	GameItemView* m_itmView[3];

	void onTabSelect( CCObject* e );
	void onPMNameChange( CCObject* e );

	void initCharView();
	void initItemsView();

	void onItemViewArmor(CCObject* e);
	void onItemViewEquip(CCObject* e);
	void onItemViewWeap(CCObject* e);

public:
	PartyMemberEditor(void);
	~PartyMemberEditor(void);
	
	static PartyMemberEditor * create( GameEntity* entity, const CCSize& size );
	bool init( GameEntity* entity, const CCSize& size );

};

#endif
