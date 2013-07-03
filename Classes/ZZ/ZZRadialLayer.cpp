#include "ZZRadialLayer.h"

namespace ZZ {

RadialLayer::RadialLayer(void)
{
	m_center = NULL;
	m_bg = NULL;
	m_radius = 0;
	m_tierRadius = 0;
}


RadialLayer::~RadialLayer(void)
{
}

//static 
RadialLayer* RadialLayer::create()
{
	RadialLayer* rl = new RadialLayer();
	if(!rl->init()) {
		CC_SAFE_DELETE(rl);
		return NULL;
	}
	rl->autorelease();
	return rl;
}

//virtual 
bool RadialLayer::init( float radius , float tierRadius  )
{
	m_radius = radius;
	m_tierRadius = tierRadius;

	m_bg = CCDrawNode::create();

	m_bg->drawDot( ccp(0,0), 150, ccc4f(0.7,0.7,0.7,0.7));
	addChild(m_bg);

	setScale(0.5);
	runAction( CCScaleTo::create(0.5f, 1,1) );

	return true;
}

void RadialLayer::setCenterNode( CCNode* node ) 
{
	if( m_center != NULL ) {
		removeChild(m_center);
		m_center = NULL;
	}

	//TODO: rather than modifying the node, should we just place it in a holder that offsets it?
	m_center = node;
	m_center->setPosition(0,0);
	m_center->setAnchorPoint(ccp(0.5,0.5));
	m_center->ignoreAnchorPointForPosition(false);
	addChild(m_center);
}

//TODO: minimum radial distance, spawn new tier/ring


void RadialLayer::addItem( CCNode* node )
{
	m_items.push_back(node);

	addChild(node);

	//rearrange positions
	float delta = 360.0f / m_items.size();

	int tier = 1;
	float tRadius = tier * m_tierRadius;
	for( int i=0 ; i< m_items.size(); i++ )
	{
		float t = CC_DEGREES_TO_RADIANS( delta * i );
		float x = (tRadius)*cos(t);
		float y = (tRadius)*sin(t);

		m_items[i]->setPosition(x,y);
	}


}

};