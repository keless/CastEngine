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

}

#endif
