#ifndef __MAINMENUSCREEN_H__
#define __MAINMENUSCREEN_H__

#include "cocos2d.h"
using namespace cocos2d;

class MainMenuScreen : public CCLayer
{
public:
	MainMenuScreen(void);
	~MainMenuScreen(void);

	// implement the "static node()" method manually
    CREATE_FUNC(MainMenuScreen);

	bool init();

    // a selector callback
    void menuStartCallback(CCObject* pSender);


};

#endif

