#ifndef _ZZTOUCHABLE_H_
#define _ZZTOUCHABLE_H_

/**************

TouchableNode is a CCLayer that will fire off a JsonEvent to 
	the EventBus when any of its children are touched.

***************/

#include "ZZEventBus.h"

namespace ZZ {

class TouchableNode : public CCLayer
{
protected:
	std::string m_evt;
	std::string m_bus;
	Json::Value m_data;

	bool m_touchStarted;

public:
	TouchableNode( std::string evt, std::string bus);
	virtual ~TouchableNode(void) {}

	void setData( Json::Value& data) { m_data = data; }
	Json::Value& getData() { return m_data; }

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	//virtual void addChild(CCNode *child, int zOrder, int tag);
};

}

#endif