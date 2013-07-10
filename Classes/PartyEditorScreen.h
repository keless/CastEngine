#ifndef _PARTYEDITORSCREEN_H_
#define _PARTYEDITORSCREEN_H_

#include "ZZUtils.h"
using namespace ZZ;

#include "PartyList.h"

class PartyEditorScreen : public CCLayer
{
	PartyList* m_partyList;

public:
	PartyEditorScreen(void);
	~PartyEditorScreen(void);
    CREATE_FUNC(PartyEditorScreen);
	bool init();
};

#endif