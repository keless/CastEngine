#ifndef _ZZUTILS_H_
#define _ZZUTILS_H_

#include <cocos2d.h>

using namespace cocos2d;

#include "json.h"
#include "ZZProgressBar.h"
#include "ZZEventBus.h"
#include "ZZRadialLayer.h"
#include "ZZTouchableNode.h"

namespace ZZ {


	
Json::Value ReadFileToJson( std::string fileName );
CCLayer* CreateSimplePopup( std::string msg, std::string btnLabel, std::string okEvt, std::string bus = "game" );
TouchableNode* CreateSimpleButton( std::string msg, std::string evt, std::string bus );

}

#endif