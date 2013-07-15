#ifndef _PARTYEDITORSCREEN_H_
#define _PARTYEDITORSCREEN_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "PartyList.h"
#include "PartyMemberEditor.h"

class PartyEditorScreen : public CCLayer
{
	PartyList* m_partyList;

	PartyMemberEditor* m_partyMemberEditor;

	void onPartyMemberSelected(CCObject* e);

public:
	PartyEditorScreen(void);
	~PartyEditorScreen(void);
    CREATE_FUNC(PartyEditorScreen);
	bool init();
};

#endif