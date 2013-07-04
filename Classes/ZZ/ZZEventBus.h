#ifndef _ZZEVENTBUS_H_
#define _ZZEVENTBUS_H_

#include <cocos2d.h>
using namespace cocos2d;

#include "json.h"

namespace ZZ {

class BaseEvent : public CCObject
{
public:
	std::string type;

	BaseEvent( std::string evtType ) : type(evtType) { this->autorelease(); }
	virtual ~BaseEvent() {}
};


class JsonEvent : public BaseEvent
{
public:
	Json::Value json;

	JsonEvent( std::string evtType ) : BaseEvent(evtType) { }
	virtual ~JsonEvent() {}
};

class EventBus
{
protected:
	static EventBus* s_gameBus;
	static std::map<std::string, EventBus*> s_busHash;

public:
	static EventBus* game();

	static EventBus* get( const char* busName );




protected:
	struct callbackPair {
		CCObject* listener;
		SEL_CallFuncO callback;
	};

	std::map< std::string, std::vector< callbackPair > > m_listeners;

public:
	EventBus(void);
	virtual ~EventBus(void);

	void addListener(std::string evtName, CCObject* listener, SEL_CallFuncO callback  );
	void remListener(std::string evtName, CCObject* listener, SEL_CallFuncO callback  );

	void dispatch(std::string evtName, CCObject* evtObject );

};

}

#endif
