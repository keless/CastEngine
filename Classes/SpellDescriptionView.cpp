#include "SpellDescriptionView.h"


SpellDescriptionView::SpellDescriptionView()
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
	//CCScrollView::initWithViewSize(area.size, NULL);
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(ccp(0,0));

	//setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
	//setDirection(kCCScrollViewDirectionNone);

	setContentSize( area.size );
	setPosition(area.origin);

	EventBus::game()->addListener("spellEditorUpdate", this, callfuncO_selector(SpellDescriptionView::onSpellEditorUpdate));

	m_text = CCLabelTTF::create("", "Arial", 18, area.size, kCCTextAlignmentLeft);
	m_text->setPosition(ccp(0,0));
	m_text->setAnchorPoint(ccp(0,0));
	addChild(m_text);

	m_btnSave = CreateSimpleButton("Save","saveSpell");
	addChild(m_btnSave);
	
	return true;
}

/***********
{
 "diagramLevel":1,
 "diagramName":"novice circle",
 "mods":[null, "glyph of something" ,null],
 "effects":["effect of derp", null]
}
************/
void SpellDescriptionView::evaluateSpell( Json::Value& json )
{
	int level = json.get("diagramLevel", 1).asInt();
	std::string dName = json.get("diagramName", "").asString();

	CCString* cStr1 = CCString::createWithFormat("Diagram Lvl %d\n%s",  level, dName.c_str());

	//Json::Value spellParts = ReadFileToJson("spellParts.json");
	const Json::Value& sp_mods = JsonManager::get()->getJson("SpellParts_Mods");
	const Json::Value& sp_effs = JsonManager::get()->getJson("SpellParts_Effects");

	float castTime = 1;
	float cooldownTime = 1; 
	float range = 1;

	m_spell = Json::Value();

	

	Json::Value effects = json.get("effects", Json::Value());
	if( effects.isArray() ) {
		m_spell["effectsOnCast"] = Json::Value();
		for( int i=0; i< effects.size(); i++) {
			std::string effName = effects[i].asString();
			Json::Value sp = sp_effs.get(effName, Json::Value());
			if( sp.isNull() ) continue;

			m_spell["effectsOnCast"].append( sp );

		}
	}

	Json::Value mods = json.get("mods", Json::Value());
	if( mods.isArray() ) 
	{
		for( int i=0; i< mods.size(); i++) {
			if( mods[i].isNull() ) continue;
			std::string modName = mods[i].asString();
			Json::Value sp = sp_mods.get(modName, Json::Value());
			if( sp.isNull() ) continue;

			int modLevel = 1; //TODO: calculate mod level based on diagram position
			int modLvlSq = modLevel*modLevel;

			std::string type = sp.get("type", "").asString();
			float modVal = sp.get("value", 0).asDouble();
			if( type.compare("range") == 0 ) {
				range += modVal * modLvlSq;
			}
			else if( type.compare("cooldownTime") == 0 ) {
				cooldownTime += modVal * modLvlSq;
				if( cooldownTime < 0 ) cooldownTime = 0;
			}
			else if( type.compare("castTime") == 0 ) {
				castTime += modVal * modLvlSq;
				if( castTime < 0 ) castTime = 0;
			}

		}
	}

	m_spell["castTime"] = castTime;
	m_spell["cooldownTime"] = cooldownTime;
	m_spell["range"] = range;

	CCString* cStr2 = CCString::createWithFormat("castTime %.2f\ncooldown %.2f\nrange %.2f", castTime, cooldownTime, range);

	std::string str3 = "";
	for( int i=0; i< m_spell["effectsOnCast"].size(); i++)
	{
		str3 += m_spell["effectsOnCast"][i].toStyledString();
	}

	CCString* cStrF = CCString::createWithFormat("%s\n%s\n%s", cStr1->getCString(), cStr2->getCString(), str3.c_str());


	m_text->setString( cStrF->getCString() );
}

void SpellDescriptionView::onSpellEditorUpdate( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if( evt == NULL ) return;

	evaluateSpell( evt->json );

}