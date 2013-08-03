#ifndef _PARTYMEMBEREDITOR_H_
#define _PARTYMEMBEREDITOR_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "GameEntity.h"
#include "GameItemView.h"

#define FILE_DEFAULT_PARTY_INVENTORY_JSON "defaultPartyInventory.json"
#define FILE_PARTY_INVENTORY_JSON "partyInventory.json"

class PartyMemberEditor : public CCLayer
{

	static int m_tabIdx;

	GameEntity* m_pEntity;

	CCNode* m_editChar;
	CCNode* m_editItems;

	GameItemView* m_itmView[3];

	std::vector<GameItem*> m_partyInv;

	void onTabSelect( CCObject* e );
	void onPMNameChange( CCObject* e );

	void initPartyInventory();
	void initCharView();
	void initItemsView();

	void onItemViewArmor(CCObject* e);
	void onItemViewEquip(CCObject* e);
	void onItemViewWeap(CCObject* e);

	RadialLayer* m_itemMenu;
	void onMenuCancel( CCObject* e );
	void onMenuArmor( CCObject* e );
	void onMenuWeap( CCObject* e );
	void onMenuEquip( CCObject* e );

	void clearItemMenu();
	void resetItemMenu();

	void doItemSwap( int type, const std::string& name );

	void savePartyInventory();

public:
	PartyMemberEditor(void);
	~PartyMemberEditor(void);
	
	static PartyMemberEditor * create( GameEntity* entity, const CCSize& size );
	bool init( GameEntity* entity, const CCSize& size );

};

#endif
