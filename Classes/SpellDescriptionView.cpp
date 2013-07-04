#include "SpellDescriptionView.h"


SpellDescriptionView::SpellDescriptionView() : CCScrollView()
{
	m_text = NULL;

}


SpellDescriptionView::~SpellDescriptionView(void)
{
	EventBus::game()->remListener("spellEditorUpdate", this, callfuncO_selector(SpellDescriptionView::onSpellEditorUpdate));
}

//static 
SpellDescriptionView* SpellDescriptionView::create( CCRect area )
{
	SpellDescriptionView* view = new SpellDescriptionView();
	if(!view->init(area) ) {
		CC_SAFE_RELEASE(view);
		return NULL;
	}
	view->autorelease();
	return view;
}
//virtual 
bool SpellDescriptionView::init( CCRect area )
{
	CCScrollView::initWithViewSize(area.size, NULL);

	setContentSize( area.size );
	setPosition(area.origin);

	EventBus::game()->addListener("spellEditorUpdate", this, callfuncO_selector(SpellDescriptionView::onSpellEditorUpdate));

	m_text = CCLabelTTF::create("", "Arial", 18, area.size, CCTextAlignment::kCCTextAlignmentLeft);
	m_text->setPosition(ccp(0,0));
	m_text->setAnchorPoint(ccp(0,0));
	addChild(m_text);

	return true;
}

void SpellDescriptionView::evaluateSpell( Json::Value& json )
{
	m_text->setString( json.toStyledString().c_str() );
}

void SpellDescriptionView::onSpellEditorUpdate( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if( evt == NULL ) return;

	evaluateSpell( evt->json );

}