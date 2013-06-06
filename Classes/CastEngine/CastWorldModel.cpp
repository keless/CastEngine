//
//  CastWorldModel.cpp
//  CastEngine
//
//  Created by Paul Zirkle on 6/5/13.
//
//

#include "CastWorldModel.h"


//static
CastWorldModel* CastWorldModel::s_instance = NULL;

//static
CastWorldModel* CastWorldModel::get()
{
	if( s_instance == NULL )
	{
		s_instance = new CastWorldModel();
	}
	
	return s_instance;
}

CastWorldModel::CastWorldModel()
{
	
}

CastWorldModel::~CastWorldModel()
{
	
}

void CastWorldModel::addEffectInTransit( ICastEntity* from, CastEffect* effect, CastTarget* targetList, double startTime, double speed   )
{
	
	if( speed == 0.0 )
	{
		addEffectInstant(from, effect, targetList, startTime);
		return;
	}
	
	if( targetList->getType() == CTT_ENTITIES )
	{
		const std::vector<ICastEntity*>& targets = targetList->getEntityList();
		for( int i=0; i< targets.size(); i++)
		{
			ICastEntity* target = targets[i];
			
			CCLOG("add effect in transit" );
			
			CastEffectPath path;
			path.from = from;
			path.to = target;
			path.speed = speed;
			path.startTime = startTime;
			path.effect = effect;
			effect->retain();
			m_effectsInTransit.push_back(path);
		}
	}else {
		//TODO: world position
	}
	
	CC_SAFE_RELEASE(effect); 
	

	//TODO: scheduler?
}

void CastWorldModel::addEffectInstant(  ICastEntity* from, CastEffect* effect, CastTarget* targetList, double startTime  )
{
	
	if( targetList->getType() == CTT_ENTITIES )
	{
		const std::vector<ICastEntity*>& targets = targetList->getEntityList();
		for( int i=0; i< targets.size(); i++)
		{
			ICastEntity* target = targets[i];
			
			CastEffectPath path;
			path.from = from;
			path.to = target;
			path.speed = 0.0f;
			path.startTime = startTime;
			path.effect = effect;
			effect->retain();

			applyEffectToTarget( path );
			
		}
	}else {
		//TODO: world position
	}


	
	CC_SAFE_RELEASE(effect); 
}


void CastWorldModel::applyEffectToTarget( CastEffectPath )
{
	CCLOG("apply effect to target");
	
	//todo
}