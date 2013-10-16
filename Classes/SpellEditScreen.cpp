#include "SpellEditScreen.h"



SpellEditScreen::SpellEditScreen(void)
{
	m_spellPageList = NULL;
	m_spellDiagram = NULL;
	
	EventBus::get("spellEdit")->addListener("pageSelected", this, callfuncO_selector(SpellEditScreen::onSpellPageSelected));
	EventBus::game()->addListener("saveSpell", this, callfuncO_selector(SpellEditScreen::onSpellSave));
	EventBus::game()->addListener("saveSpellNamed", this, callfuncO_selector(SpellEditScreen::onSpellSaveNamed));
}


SpellEditScreen::~SpellEditScreen(void)
{
	EventBus::get("spellEdit")->remListener("pageSelected", this, callfuncO_selector(SpellEditScreen::onSpellPageSelected));
	EventBus::game()->remListener("saveSpell", this, callfuncO_selector(SpellEditScreen::onSpellSave));
	EventBus::game()->remListener("saveSpellNamed", this, callfuncO_selector(SpellEditScreen::onSpellSaveNamed));
}


/*

edit one individual spell

|   ^   |         |    ^    |
| page1 |         | effect1 |
|       | diagram | effect2 |
| page2 |         | effect3 |
|   V   |         |    V    |

*/

bool SpellEditScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
 
	m_spellDiagram = NULL;

	m_spellPageList = SpellPageList::create();
	m_spellPageList->setAnchorPoint(ccp(0,0));
	m_spellPageList->setPosition(0,0);
	addChild(m_spellPageList);

	m_spellDiagram = SpellDiagramNode::create();
	m_spellDiagram->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	addChild(m_spellDiagram);

	CCLayerColor* panel = CCLayerColor::create( ccc4(240,227,132,125), visibleSize.width/5, visibleSize.height);
	panel->setAnchorPoint(ccp(1,0));
	panel->ignoreAnchorPointForPosition(false);
	panel->setPosition(visibleSize.width, 0);
	addChild(panel);

	m_spellDescription = SpellDescriptionView::create(panel->boundingBox());
	addChild(m_spellDescription);

	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

void SpellEditScreen::onSpellPageSelected( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(evt == NULL) return;

	m_spellDiagram->setDiagram( evt->json.get("idx",-1).asInt() );
	//m_spellDiagram->setDiagram( (SpellDiagrams) (SD_01_NOVICE_CIRCLE + evt->json["idx"].asInt()) );

}

void SpellEditScreen::onSpellSave(CCObject* e)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCNode* pop = CreateSimpleEditBox("must enter a name", "spell name", "Save", "saveSpellNamed");
	pop->setPosition( visibleSize.width/2, visibleSize.height/2);
	addChild(pop);
}

void SpellEditScreen::onSpellSaveNamed(CCObject* e)
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if( evt == NULL ) return;

	std::string spellName = evt->json.get("string","").asString();
	if( spellName.size() < 4 ) {
		CreateSimplePopup("Spell name must be at least four characters long.", "ok", "");
		return;
	}

	Json::Value saves = ReadFileToJson("playerSpells.json");
	saves[spellName] = m_spellDescription->getSpellJson();
	WriteJsonToFile(saves, "playerSpells.json");

}