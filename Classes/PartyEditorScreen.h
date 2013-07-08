#ifndef _PARTYEDITORSCREEN_H_
#define _PARTYEDITORSCREEN_H_

#include "ZZUtils.h"
using namespace ZZ;

class PartyEditorScreen : public CCLayer
{
public:
	PartyEditorScreen(void);
	~PartyEditorScreen(void);
    CREATE_FUNC(PartyEditorScreen);
	bool init();
};

#endif