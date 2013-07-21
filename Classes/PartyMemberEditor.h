#ifndef _PARTYMEMBEREDITOR_H_
#define _PARTYMEMBEREDITOR_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "GameEntity.h"

class PartyMemberEditor : public CCLayer
{

	GameEntity* m_pEntity;

	CCNode* m_editChar;
	CCNode* m_editItems;

	void onTabSelect( CCObject* e );
	void onPMNameChange( CCObject* e );

	void initCharView();

public:
	PartyMemberEditor(void);
	~PartyMemberEditor(void);
	
	static PartyMemberEditor * create( GameEntity* entity, const CCSize& size );
	bool init( GameEntity* entity, const CCSize& size );

};

#endif
