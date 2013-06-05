#include "CastCommandModel.h"


CastCommandModel::CastCommandModel(Json::Value castData)
{
	descriptor = castData;

	name = descriptor.get("name", "effectName").asString();
	castTime = descriptor.get("castTime", 0.0f).asDouble();
	channelTime = descriptor.get("channelTime", 0.0f).asDouble();
	travelSpeed = descriptor.get("travelSpeed", 0.0f).asDouble();
	range = descriptor.get("range", 0.0f).asDouble();
	effectWhileTraveling = descriptor.get("effectWhileTravel", false).asBool();
	stopOnHit = descriptor.get("stopOnHit", true).asBool();
	effectSize = descriptor.get("effectSize", 0.0f).asDouble();
	cooldownTime = descriptor.get("cooldownTime", 0.0f).asDouble();
	
	effectsOnCast = castData.get("effectsOnCast", Json::Value());
}


CastCommandModel::~CastCommandModel(void)
{
}
