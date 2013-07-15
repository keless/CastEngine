#ifndef _PARTYMEMBEREDITOR_H_
#define _PARTYMEMBEREDITOR_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "GameEntity.h"

class PartyMemberEditor : public CCLayer
{

	GameEntity* m_pEntity;

	void onTabSelect( CCObject* e );
	void onPMNameChange( CCObject* e );

public:
	PartyMemberEditor(void);
	~PartyMemberEditor(void);
	
	static PartyMemberEditor * create( GameEntity* entity, const CCSize& size );
	bool init( GameEntity* entity, const CCSize& size );

};

#endif
