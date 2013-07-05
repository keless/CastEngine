#ifndef __SPELLEDITSCREEN_H__
#define __SPELLEDITSCREEN_H__

#include "ZZUtils.h"
using namespace ZZ;

#include "SpellPageList.h"
#include "SpellDiagramNode.h"
#include "SpellDescriptionView.h"

class SpellEditScreen : public CCLayer
{
	SpellPageList* m_spellPageList;

	SpellDiagramNode* m_spellDiagram;

	SpellDescriptionView* m_spellDescription;

public:
	SpellEditScreen(void);
	~SpellEditScreen(void);
    CREATE_FUNC(SpellEditScreen);
	bool init();

	void onSpellPageSelected( CCObject* e );

	void onSpellSave( CCObject* e );
};

#endif
