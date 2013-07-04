
#ifndef _SPELLDESCRIPTIONVIEW_H_
#define _SPELLDESCRIPTIONVIEW_H_


#include "ZZUtils.h"
using namespace ZZ;

#include "cocos-ext.h"
using namespace extension;


class SpellDescriptionView : public CCScrollView, public CCScrollViewDelegate
{
	CCLabelTTF* m_text;

	Json::Value m_spell;

public:
	SpellDescriptionView();
	~SpellDescriptionView(void);

	static SpellDescriptionView* create( CCRect area );
	virtual bool init( CCRect area );  

	void evaluateSpell( Json::Value& json );

	void onSpellEditorUpdate( CCObject* e );

	virtual void scrollViewDidScroll(CCScrollView* view) {}
	virtual void scrollViewDidZoom(CCScrollView* view) {}
};

#endif
