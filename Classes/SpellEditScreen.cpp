#include "SpellEditScreen.h"



SpellEditScreen::SpellEditScreen(void)
{
	m_spellPageList = NULL;
	m_spellDiagram = NULL;
	
	EventBus::get("spellEdit")->addListener("pageSelected", this, callfuncO_selector(SpellEditScreen::onSpellPageSelected));
}


SpellEditScreen::~SpellEditScreen(void)
{
	EventBus::get("spellEdit")->remListener("pageSelected", this, callfuncO_selector(SpellEditScreen::onSpellPageSelected));
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
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
 
	m_spellDiagram = NULL;

	m_spellPageList = SpellPageList::create();
	m_spellPageList->setAnchorPoint(ccp(0,0));
	m_spellPageList->setPosition(0,0);
	addChild(m_spellPageList);

	CCLayerColor* panel = CCLayerColor::create( ccc4(240,227,132,125), visibleSize.width/5, visibleSize.height);
	panel->setAnchorPoint(ccp(1,0));
	panel->ignoreAnchorPointForPosition(false);
	panel->setPosition(visibleSize.width, 0);
	addChild(panel);

	m_spellDiagram = SpellDiagramNode::create();
	m_spellDiagram->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	addChild(m_spellDiagram);

	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

void SpellEditScreen::onSpellPageSelected( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	//if(!evt) return;


	m_spellDiagram->setDiagram( (SpellDiagrams) (SD_01_NOVICE_CIRCLE + evt->json["idx"].asInt()) );

}