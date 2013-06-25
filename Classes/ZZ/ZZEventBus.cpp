#include "ZZEventBus.h"


//static 
ZZEventBus* ZZEventBus::s_gameBus = NULL;
//static 
std::map<std::string, ZZEventBus*> ZZEventBus::s_busHash;

//static 
ZZEventBus* ZZEventBus::game()
{
	if( s_gameBus == NULL ) {
		s_gameBus = new ZZEventBus();
	}
	return s_gameBus;
}

//static 
ZZEventBus* ZZEventBus::get( char* busName )
{
	if( s_busHash.count(busName) < 1 ) 
	{
		s_busHash[busName] = new ZZEventBus();
	}

	return s_busHash[busName];
}

ZZEventBus::ZZEventBus(void)
{
}


ZZEventBus::~ZZEventBus(void)
{
}

void ZZEventBus::addListener(std::string evtName, CCObject* listener, SEL_CallFuncO callback  )
{
	callbackPair cbp;
	cbp.callback = callback;
	cbp.listener = listener;

	if( m_listeners.count( evtName ) == 0 ) {
		m_listeners[evtName] = std::vector<callbackPair>();
	}

	m_listeners[evtName].push_back(cbp);
}

void ZZEventBus::remListener(std::string evtName, CCObject* listener, SEL_CallFuncO callback  )
{
	if( m_listeners.count( evtName ) == 0 ) {
		return;
	}

	std::vector<callbackPair> & listeners = m_listeners[evtName];
	for( int i=0; i< (int) listeners.size(); i++) 
	{
		if( listeners[i].callback == callback && listeners[i].listener == listener )
		{
			listeners.erase( listeners.begin() + i );
			return;
		}
	}
}

void ZZEventBus::dispatch(std::string evtName, CCObject* evtObject )
{
	std::vector<callbackPair> & listeners = m_listeners[evtName];
	for( int i=0; i< (int) listeners.size(); i++) 
	{
		
		CCCallFuncO* cfunc = CCCallFuncO::create(listeners[i].listener, listeners[i].callback, evtObject);
		cfunc->execute();
		CC_SAFE_RELEASE_NULL(cfunc);

	}

	CCLog("dispatch %s complete", evtName.c_str());
}