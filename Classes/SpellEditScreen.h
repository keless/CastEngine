#ifndef __SPELLEDITSCREEN_H__
#define __SPELLEDITSCREEN_H__

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

#include "SpellPageList.h"
#include "SpellDiagramNode.h"

#include "ZZEventBus.h"
using namespace ZZ;

class SpellEditScreen : public CCLayer
{
	SpellPageList* m_spellPageList;

	SpellDiagramNode* m_spellDiagram;

public:
	SpellEditScreen(void);
	~SpellEditScreen(void);
    CREATE_FUNC(SpellEditScreen);
	bool init();

	void onSpellPageSelected( CCObject* e );
};

#endif
