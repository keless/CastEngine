#ifndef _ZZEVENTBUS_H_
#define _ZZEVENTBUS_H_

#include <cocos2d.h>
using namespace cocos2d;

class ZZEventBus
{
protected:
	static ZZEventBus* s_gameBus;
public:
	static ZZEventBus* game();

protected:
	struct callbackPair {
		CCObject* listener;
		SEL_CallFuncO callback;
	};

	std::map< std::string, std::vector< callbackPair > > m_listeners;

public:
	ZZEventBus(void);
	~ZZEventBus(void);

	void addListener(std::string evtName, CCObject* listener, SEL_CallFuncO callback  );
	void remListener(std::string evtName, CCObject* listener, SEL_CallFuncO callback  );

	void dispatch(std::string evtName, CCObject* evtObject );

};


#endif
