#ifndef _ZZTOUCHABLE_H_
#define _ZZTOUCHABLE_H_

#include "ZZEventBus.h"

namespace ZZ {

	class TouchableNode : public CCLayer
{
protected:
	std::string m_evt;
	std::string m_bus;

	bool m_touchStarted;

public:
	TouchableNode( std::string evt, std::string bus);
	virtual ~TouchableNode(void) {}

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	//virtual void addChild(CCNode *child, int zOrder, int tag);
};

}

#endif