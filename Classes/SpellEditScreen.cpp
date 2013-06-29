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
 


	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}