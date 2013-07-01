#include "SpellDiagramNode.h"


SpellDiagramNode::SpellDiagramNode(void)
{
	m_type = SD_INVALID;
}


SpellDiagramNode::~SpellDiagramNode(void)
{
}

bool SpellDiagramNode::init()
{
	setAnchorPoint(ccp(0.5f,0.5f));
	//setContentSize(CCSizeMake(100,100));

	CCParticleSpiral* ps = CCParticleSpiral::createWithTotalParticles(150);
	ps->setPosition(ccp(0,0));
	ps->setScale(0.6);
	ps->setStartColor(ccc4f(0.1,0.0,1.0,1.0f));
	ps->setStartColorVar(ccc4f(0.1,0.0,0.2,0.0));
	ps->setEndColor(ccc4f(0.2,0.0,0.9,0.5f));
	ps->setEndColorVar(ccc4f(0.1,0.0,0.2,0.0));
	addChild(ps);


	m_ps = ps;
	m_ps->stopSystem();

	/*
	CCParticleFire* fire = CCParticleFire::createWithTotalParticles(150);
	CCPoint point = CCPointMake(  m_healthBar->getContentSize().width/2, 0 );

	//fire->stopSystem();
	fire->setSpeed(20);
	fire->setStartSize(25);
	CCPoint posVar = fire->getPosVar();
	posVar.y /= 2;
	fire->setPosVar( posVar );

	fire->setPosition( point );
	addChild(fire);
	//*/

	return true;
}

void SpellDiagramNode::setDiagram( SpellDiagrams diagram )
{
	if( m_type == SD_INVALID && diagram != SD_INVALID ) 
	{
		//m_ps->resetSystem();
	}else if( diagram == SD_INVALID ) {
		m_ps->stopSystem();
	}


	m_type = diagram;

}

//virtual 
void SpellDiagramNode::draw()
{
	CCNode::draw();

	float size = 400;
	float ptSz = 5;

	ccDrawInit();
	ccPointSize(ptSz);
	glLineWidth(ptSz);
	//ccDrawColor4B(0, 0, 256, 256);
	ccDrawColor4F(0.0f, 0.0f, 1.0f, 1.0f);
    //ccDrawLine(ccp(0,0), ccp(100, 100));

	CCPoint tA = ccp(0, size*0.5); //top
	CCPoint tB = ccp(size*0.4330, -size*0.25); //br
	CCPoint tC = ccp(-size*0.4330, -size*0.25); //bl

	if( m_type == SD_01_NOVICE_CIRCLE || m_type == SD_03_ADEPTS_CIRCLE || 
		m_type == SD_07_COMPASS || m_type == SD_13_GREATER_PYRAMID || 
		m_type == SD_14_GREATER_TRIQUETRA ) 
	{
		//circle shape
		ccDrawCircle(ccp(0,0), size/2, 0, 32, false);

		/* modulate circle
		struct cc_timeval now;
		CCTime::gettimeofdayCocos2d(&now, NULL);
		float co = cos( now.tv_usec / 100000 );

		ccDrawColor4F(0.2f, 0.0f, 1.0f, 1.0f);
		ccDrawCircle(ccp(0,0), size/2 + 4 *co, 0, 32, false);
		*/
	}
	
	if( m_type == SD_02_BLIND_EYE || m_type == SD_05_SERPENTS_EYE || 
		m_type == SD_09_DRAGONS_EYE || m_type == SD_10_SEEING_EYE ) 
	{
		//eye shape
		ccDrawCubicBezier(ccp(-size/2, 0), ccp(-size/3, size/2), ccp(size/3,size/2), ccp(size/2,0), 32);
		ccDrawCubicBezier(ccp(-size/2, 0), ccp(-size/3, -size/2), ccp(size/3,-size/2), ccp(size/2,0), 32);
		//ccDrawQuadBezier(ccp(-size/2, 0), ccp(0, size), ccp(size/2, 0), 32);
	}

	if( m_type == SD_04_LESSER_PYRAMID || m_type == SD_11_SCHOOL || 
		m_type == SD_13_GREATER_PYRAMID ) 
	{
		//triangle shape
		ccDrawLine(tA, tC); //left
		ccDrawLine(tA, tB); //right
		ccDrawLine(tB, tC); //base
	}

	if( m_type == SD_08_FORTRESS )  {
		//square shape
		ccDrawLine(ccp(-size/2 - ptSz/2, size/2), ccp(size/2 + ptSz/2, size/2)); //top
		ccDrawLine(ccp(-size/2 - ptSz/2, -size/2), ccp(size/2 + ptSz/2, -size/2)); //btm

		ccDrawLine(ccp(size/2, size/2), ccp(size/2 , -size/2)); //right
		ccDrawLine(ccp(-size/2, size/2), ccp(-size/2 , -size/2)); //left
	}

	if( m_type == SD_06_LESSER_TRIQUESTRA || m_type == SD_12_LEAF || 
		m_type == SD_14_GREATER_TRIQUETRA ) 
	{
		CCPoint lcp1 = ccp(size*0.1666, -size*0.4167);
		CCPoint lcp2 = ccp(size*0.3335, -size*0.0833);
		ccDrawCubicBezier(ccp(-size/2, -size/2), lcp1, lcp2, ccp(0,size/2), 32); //left

		CCPoint rcp1 = ccp(-size*0.1666, -size*0.4167);
		CCPoint rcp2 = ccp(-size*0.3335, -size*0.0833);
		ccDrawCubicBezier(ccp(size/2, -size/2), rcp1, rcp2, ccp(0,size/2), 32); //right

		CCPoint bcp1 = ccp(-size*0.25, size*0.166);
		CCPoint bcp2 = ccp( size*0.25, size*0.166);
		ccDrawCubicBezier(ccp(-size/2, -size/2), bcp1, bcp2, ccp(size/2,-size/2), 32); //bottom

		ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
				ccDrawPoint( lcp1);
		ccDrawPoint(lcp2);
				ccDrawPoint(rcp1);
		ccDrawPoint(rcp2);
		ccDrawPoint(bcp1);
		ccDrawPoint(bcp2);
		ccDrawColor4F(0.0f, 0.0f, 1.0f, 1.0f);
	}

	if( m_type == SD_07_COMPASS || m_type == SD_03_ADEPTS_CIRCLE ) {
		ccDrawLine(ccp(0, size/2), ccp(0, -size/2)); 
	}

	if( m_type == SD_11_SCHOOL ) {
		//verticle line
		ccDrawLine(tA, ccp(0, tB.y)); 
	}

	if( m_type == SD_05_SERPENTS_EYE ) {
		//shorter verticle line for eye
		ccDrawLine(ccp(0, size*0.38), ccp(0, -size*0.38));
	}

	if( m_type == SD_12_LEAF ) {
		//offset verticle line for leaf
		ccDrawLine(ccp(0, size*0.5), ccp(0, -size*0.38));
	}

	if( m_type == SD_09_DRAGONS_EYE ) {
		//shorter eye
		CCPoint t = ccp(0, size*0.38);
		CCPoint b = ccp(0, -size*0.38);
		CCPoint rcp1 = ccp(size/3,size/5);
		CCPoint rcp2 = ccp(size/3,-size/5);
		ccDrawCubicBezier(t, rcp1, rcp2, b, 32);
		CCPoint lcp1 = ccp(-size/3,size/5);
		CCPoint lcp2 = ccp(-size/3,-size/5);
		ccDrawCubicBezier(t, lcp1, lcp2, b, 32);
	}

	if( m_type == SD_10_SEEING_EYE ) {
		//shorter eye
		ccDrawCubicBezier(ccp(-size/2, 0), ccp(-size/3, size/4), ccp(size/3,size/4), ccp(size/2,0), 32);
		ccDrawCubicBezier(ccp(-size/2, 0), ccp(-size/3, -size/4), ccp(size/3,-size/4), ccp(size/2,0), 32);
	}

	if( m_type == SD_07_COMPASS  ) {
		//horizontal line
		ccDrawLine(ccp(-size/2, 0), ccp(size/2, 0));
	}
}

