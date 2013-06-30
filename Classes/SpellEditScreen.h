#ifndef __SPELLEDITSCREEN_H__
#define __SPELLEDITSCREEN_H__

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class SpellEditScreen : public CCLayer
{
public:
	SpellEditScreen(void);
	~SpellEditScreen(void);
    CREATE_FUNC(SpellEditScreen);
	bool init();


};

#endif
