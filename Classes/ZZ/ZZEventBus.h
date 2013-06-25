#ifndef _ZZEVENTBUS_H_
#define _ZZEVENTBUS_H_

#include <cocos2d.h>
using namespace cocos2d;

class ZZEventBus
{
protected:
	static ZZEventBus* s_gameBus;
	static std::map<std::string, ZZEventBus*> s_busHash;

public:
	static ZZEventBus* game();

	static ZZEventBus* get( char* busName );

	class BaseEvent : public CCObject
	{
	public:
		std::string type;

		BaseEvent( std::string evtType ) : type(evtType) { this->autorelease(); }
	};


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
