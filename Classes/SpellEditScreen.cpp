#include "SpellEditScreen.h"



SpellEditScreen::SpellEditScreen(void)
{
}


SpellEditScreen::~SpellEditScreen(void)
{
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
 

	CCLayerColor* panel = CCLayerColor::create( ccc4(240,227,132,125), visibleSize.width/5, visibleSize.height);
	panel->setAnchorPoint(ccp(0,0));
	panel->setPosition(0,0);
	addChild(panel);

	panel = CCLayerColor::create( ccc4(240,227,132,125), visibleSize.width/5, visibleSize.height);
	panel->setAnchorPoint(ccp(1,0));
	panel->ignoreAnchorPointForPosition(false);
	panel->setPosition(visibleSize.width, 0);
	addChild(panel);


	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}