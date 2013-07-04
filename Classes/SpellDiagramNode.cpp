#include "SpellDiagramNode.h"

#define DSIZE 400

SpellDiagramNode::SpellDiagramNode(void)
{
	m_type = SD_INVALID;
	m_slotEquipMenu = NULL; 
	
	m_modSelectedIdx = -1;
	m_effSelectedIdx = -1;
}


SpellDiagramNode::~SpellDiagramNode(void)
{
}

CCDrawNode* createPentNode( ccColor4F fill, ccColor4F outline)
{
	CCPoint pent[5];
	pent[0] = CCPointMake(0,10);
	pent[1] = CCPointMake(10,2);
	pent[2] = CCPointMake(5,-7);
	pent[3] = CCPointMake(-5,-7);
	pent[4] = CCPointMake(-10,2);

	CCDrawNode* pt = CCDrawNode::create();
	pt->drawPolygon(pent, 5, fill, 1, outline);
	return pt;
}

bool SpellDiagramNode::init()
{
	setAnchorPoint(ccp(0.5f,0.5f));
	ignoreAnchorPointForPosition(false);
	//setContentSize(CCSizeMake(100,100));
	size = DSIZE;

	CCParticleSpiral* ps = CCParticleSpiral::createWithTotalParticles(150);
	ps->setPosition(ccp(0,0));
	ps->setScale(0.6);
	ps->setStartColor(ccc4f(0.1,0.0,1.0,1.0f));
	ps->setStartColorVar(ccc4f(0.1,0.0,0.2,0.0));
	ps->setEndColor(ccc4f(0.2,0.0,0.9,0.5f));
	ps->setEndColorVar(ccc4f(0.1,0.0,0.2,0.0));
	addChild(ps);

	EventBus::game()->addListener("slotMenuCancel", this, callfuncO_selector(SpellDiagramNode::onMenuCancel));
	EventBus::game()->addListener("slotMenuModHeal", this, callfuncO_selector(SpellDiagramNode::onMenuMod));
	EventBus::game()->addListener("slotMenuModDamage", this, callfuncO_selector(SpellDiagramNode::onMenuMod));

	m_ps = ps;
	m_ps->stopSystem();

	setTouchEnabled(true);

	return true;
}

void SpellDiagramNode::onMenuCancel( CCObject* e )
{
	if( m_slotEquipMenu != NULL ) {
		m_slotEquipMenu->removeFromParentAndCleanup(true);
		m_slotEquipMenu = NULL;
	}
}
void SpellDiagramNode::onMenuMod(CCObject* e )
{
	if( m_slotEquipMenu != NULL && m_modSelectedIdx >= 0 ) {
		//handle mod selection

		//clean up menu
		m_slotEquipMenu->removeFromParentAndCleanup(true);
		m_slotEquipMenu = NULL;
	}
}
void SpellDiagramNode::onMenuEff(CCObject* e )
{
	if( m_slotEquipMenu != NULL && m_effSelectedIdx >= 0 ) {
		//handle eff selection

		//clean up menu
		m_slotEquipMenu->removeFromParentAndCleanup(true);
		m_slotEquipMenu = NULL;
	}
}

#define TRANSITION_TIME 0.5f
#define MOD_COLOR ccc4f(0,1,0,1)
#define EFF_COLOR ccc4f(1,0,0,1)

void SpellDiagramNode::addEffect( int idx, float x, float y, int level ) {


	if( idx < m_effectSlots.size() )
	{
		CCDrawNode* pt = m_effectSlots[idx];
		pt->runAction( CCMoveTo::create(TRANSITION_TIME, ccp(x,y)) );
	}else {
		//create new
		CCDrawNode* pt = createPentNode(EFF_COLOR, ccc4f(0,0,0,1));
		pt->setPosition(x,y);
		addChild(pt);
		pt->setScale(0.01);
		pt->runAction(CCScaleTo::create(TRANSITION_TIME/2, 1,1));
		m_effectSlots.push_back(pt);
	}
}



void SpellDiagramNode::addMod( int idx, float x, float y, int level ) {

	if( idx < m_modSlots.size() )
	{
		CCDrawNode* pt = m_modSlots[idx];
		pt->runAction( CCMoveTo::create(TRANSITION_TIME, ccp(x,y)) );
	}else {

		CCDrawNode* pt = createPentNode(MOD_COLOR, ccc4f(0,0,0,1));
		pt->setPosition(x,y);
		addChild(pt);
		pt->setScale(0.01);
		pt->runAction(CCScaleTo::create(TRANSITION_TIME/2, 1,1));
		m_modSlots.push_back(pt);
	}
}

void SpellDiagramNode::trimEffectsSize( int maxEffects )
{
	if( m_effectSlots.size() <= maxEffects ) return;

	for( int i= maxEffects; i < m_effectSlots.size(); i++ ) {
		m_effectSlots[i]->removeFromParentAndCleanup(true);
	}
	m_effectSlots.erase( m_effectSlots.begin() + maxEffects, m_effectSlots.end() );

}
void SpellDiagramNode::trimModsSize( int maxMods )
{
	
	if( m_modSlots.size() <= maxMods ) return;

	for( int i= maxMods; i < m_modSlots.size(); i++ ) {
		m_modSlots[i]->removeFromParentAndCleanup(true);
	}
	m_modSlots.erase( m_modSlots.begin() + maxMods, m_modSlots.end() );
}

//triangle
CCPoint tA = ccp(0, DSIZE*0.5); //top
CCPoint tB = ccp(DSIZE*0.4330, -DSIZE*0.25); //br
CCPoint tC = ccp(-DSIZE*0.4330, -DSIZE*0.25); //bl
//serpents eye
CCPoint seT = ccp(0, DSIZE*0.38);
CCPoint seB = ccp(0, -DSIZE*0.38);
//leaf
CCPoint leT = ccp(0, DSIZE*0.5);
CCPoint leB = ccp(0, -DSIZE*0.15);

void SpellDiagramNode::setDiagram( SpellDiagrams diagram )
{
	if( m_type == SD_INVALID && diagram != SD_INVALID ) 
	{
		//m_ps->resetSystem();
	}else if( diagram == SD_INVALID ) {
		m_ps->stopSystem();
	}


	m_type = diagram;

	CCDrawNode* pt = NULL;
	switch( m_type ) {
	case SD_01_NOVICE_CIRCLE:
		trimEffectsSize(1);
		trimModsSize(0);
		addEffect(0, 0,0, 1);
		break;
	case SD_02_BLIND_EYE:
		trimEffectsSize(1);
		trimModsSize(2);
		addEffect(0, 0,0, 2);
		addMod(0, -size*0.5,0, 2);
		addMod(1,  size*0.5,0, 2);
		break;
	case SD_03_ADEPTS_CIRCLE:
		trimEffectsSize(2);
		trimModsSize(2);
		addEffect(0, -size*0.25,0, 2);
		addEffect(1,  size*0.25,0, 2);
		addMod(0, 0,-size*0.5, 3);
		addMod(1, 0, size*0.5, 3);
		break;

	case SD_04_LESSER_PYRAMID:
		trimEffectsSize(1);
		trimModsSize(3);
		addEffect(0, 0,0, 3);
		addMod(0, tA.x,tA.y, 2);
		addMod(1, tB.x,tB.y, 2);
		addMod(2, tC.x,tC.y, 2);
		break;

	case SD_05_SERPENTS_EYE:
		trimEffectsSize(2);
		trimModsSize(4);
		addEffect(0, -size*0.25,0, 3);
		addEffect(1,  size*0.25,0, 3);
		addMod(0, seT.x,seT.y, 3);
		addMod(1, seB.x,seB.y, 3);
		addMod(2, -size*0.5,0, 2);
		addMod(3,  size*0.5,0, 2);
		break;

	case SD_06_LESSER_TRIQUESTRA:
		trimEffectsSize(4);
		trimModsSize(6);
		addEffect(0, 0,size*0.02, 3);
		addEffect(1, 0,size*0.25, 3);
		addEffect(2, -size*0.25,-size*0.1, 3);
		addEffect(3,  size*0.25,-size*0.1, 3);
		addMod(0, -size*0.1667,size*0.1, 4);
		addMod(1,  size*0.1667,size*0.1, 4);
		addMod(2,  0,-size*0.15, 4);
		addMod(3, tA.x,tA.y, 2);
		addMod(4, tB.x,tB.y, 2);
		addMod(5, tC.x,tC.y, 2);
		break;

	case SD_07_COMPASS:
		trimEffectsSize(4);
		trimModsSize(5);
		addEffect(0, -size*0.25, size*0.25, 3);
		addEffect(1,  size*0.25, size*0.25, 3);
		addEffect(2,  size*0.25,-size*0.25, 3);
		addEffect(3, -size*0.25,-size*0.25, 3);

		addMod(0, 0,0, 4);
		addMod(1, 0, size*0.5, 3);
		addMod(2, size*0.5,0, 3);
		addMod(3, 0,-size*0.5, 3);
		addMod(4, -size*0.5,0, 3);
		break;

	case SD_08_FORTRESS:
		trimEffectsSize(1);
		trimModsSize(4);
		addEffect(0, 0,0, 4);
		addMod(0, -size*0.5, size*0.5, 2);
		addMod(1,  size*0.5, size*0.5, 2);
		addMod(2,  size*0.5,-size*0.5, 2);
		addMod(3, -size*0.5,-size*0.5, 2);
		break;

	case SD_09_DRAGONS_EYE:
		trimEffectsSize(3);
		trimModsSize(4);
		addEffect(0, 0,0, 2);
		addEffect(1, -size*0.375,0, 2);
		addEffect(2,  size*0.375,0, 2);

		addMod(0, seT.x,seT.y, 4);
		addMod(1, seB.x,seB.y, 4);
		addMod(2, -size*0.5,0, 2);
		addMod(3,  size*0.5,0, 2);
		break;

	case SD_10_SEEING_EYE:
		trimEffectsSize(3);
		trimModsSize(2);
		addEffect(0, 0,0, 2);
		addEffect(1, 0, size*0.375, 2);
		addEffect(2, 0,-size*0.375, 2);


		addMod(0, -size*0.5,0, 2);
		addMod(1,  size*0.5,0, 2);
		break;

	case SD_11_LEAF:
		trimEffectsSize(6);
		trimModsSize(7);
		addEffect(0, -size*0.075,size*0.02, 3);
		addEffect(1, -size*0.075,size*0.25, 3);
		addEffect(2,  size*0.075,size*0.02, 3);
		addEffect(3,  size*0.075,size*0.25, 3);
		addEffect(4, -size*0.25,-size*0.1, 3);
		addEffect(5,  size*0.25,-size*0.1, 3);
		addMod(0, -size*0.1667,size*0.1, 4);
		addMod(1,  size*0.1667,size*0.1, 4);
		addMod(2, 0,-size*0.15, 4);
		addMod(3, 0,size*0.15, 2);
		addMod(4, tA.x,tA.y, 2);
		addMod(5, tB.x,tB.y, 2);
		addMod(6, tC.x,tC.y, 2);

		break;
	
	case SD_12_GREATER_PYRAMID:
		trimEffectsSize(4);
		trimModsSize(3);
		addEffect(0, 0,0, 3);
		addEffect(1, -size*0.3,size*0.20, 3);
		addEffect(2,  size*0.3,size*0.20, 3);
		addEffect(3,  0,-size*0.35, 3);
		addMod(0, tA.x,tA.y, 4);
		addMod(1, tB.x,tB.y, 4);
		addMod(2, tC.x,tC.y, 4);
		break;

	case SD_13_GREATER_TRIQUETRA:
		trimEffectsSize(7);
		trimModsSize(6);
		addEffect(0, 0,size*0.02, 3);
		addEffect(1, 0,size*0.25, 3);
		addEffect(2, -size*0.25,-size*0.1, 3);
		addEffect(3,  size*0.25,-size*0.1, 3);


		addEffect(4, -size*0.3,size*0.20, 3);
		addEffect(5,  size*0.3,size*0.20, 3);
		addEffect(6,  0,-size*0.35, 3);

		addMod(0, -size*0.1667,size*0.1, 4);
		addMod(1,  size*0.1667,size*0.1, 4);
		addMod(2,  0,-size*0.15, 4);
		addMod(3, tA.x,tA.y, 4);
		addMod(4, tB.x,tB.y, 4);
		addMod(5, tC.x,tC.y, 4);
		break;

	default:
		trimEffectsSize(0);
		trimModsSize(0);
	}

}

//virtual 
void SpellDiagramNode::draw()
{
	CCNode::draw();

	float ptSz = 5;

	ccDrawInit();
	ccPointSize(ptSz);
	glLineWidth(ptSz);
	//ccDrawColor4B(0, 0, 256, 256);
	ccDrawColor4F(0.0f, 0.0f, 1.0f, 1.0f);
    //ccDrawLine(ccp(0,0), ccp(100, 100));

	if( m_type == SD_01_NOVICE_CIRCLE || m_type == SD_03_ADEPTS_CIRCLE || 
		m_type == SD_07_COMPASS || m_type == SD_12_GREATER_PYRAMID || 
		m_type == SD_13_GREATER_TRIQUETRA ) 
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

	if( m_type == SD_04_LESSER_PYRAMID || m_type == SD_12_GREATER_PYRAMID ) 
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

	if( m_type == SD_06_LESSER_TRIQUESTRA || m_type == SD_11_LEAF || 
		m_type == SD_13_GREATER_TRIQUETRA ) 
	{
		float topY = tA.y;
		float botY = tB.y;
		float midY = 0;

		CCPoint lcp1 = ccp(size*0.1666, -size*0.2167);
		CCPoint lcp2 = ccp(size*0.3335, size*0.1233);
		ccDrawCubicBezier(tC, lcp1, lcp2, tA, 32); //left

		CCPoint rcp1 = ccp(-size*0.1666, -size*0.2167);
		CCPoint rcp2 = ccp(-size*0.3335, size*0.1233);
		ccDrawCubicBezier(tB, rcp1, rcp2, tA, 32); //right

		CCPoint bcp1 = ccp(-size*0.25, size*0.286);
		CCPoint bcp2 = ccp( size*0.25, size*0.286);
		ccDrawCubicBezier(tC, bcp1, bcp2, tB, 32); //bottom

		/*
		ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
		ccDrawPoint( lcp1);
		ccDrawPoint(lcp2);
		ccDrawPoint(rcp1);
		ccDrawPoint(rcp2);
		ccDrawPoint(bcp1);
		ccDrawPoint(bcp2);
		ccDrawColor4F(0.0f, 0.0f, 1.0f, 1.0f);
		*/
	}

	if( m_type == SD_07_COMPASS || m_type == SD_03_ADEPTS_CIRCLE ) {
		ccDrawLine(ccp(0, size/2), ccp(0, -size/2)); 
	}

	if( m_type == SD_05_SERPENTS_EYE ) {
		//shorter verticle line for eye
		ccDrawLine( seT, seB);
	}

	if( m_type == SD_11_LEAF ) {
		//offset verticle line for leaf
		ccDrawLine(leT, leB);
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

void SpellDiagramNode::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	float slotRadius = 25.0f;

	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		if( m_slotEquipMenu == NULL ) {

			CCPoint p = touch->getLocation();
			p.x -= getPositionX();
			p.y -= getPositionY();

			for(int i=0; i< m_effectSlots.size(); i++)
			{
				CCPoint sp = m_effectSlots[i]->getPosition();

				if( p.getDistance(sp) <= slotRadius ) {
					CCLog("touched effect %d", i);

					m_slotEquipMenu = RadialLayer::create();
					m_modSelectedIdx = i;
					
					m_slotEquipMenu->setCenterNode(createPentNode(EFF_COLOR, ccc4f(0,0,0,1)));
					m_slotEquipMenu->setPosition( sp );
					addChild(m_slotEquipMenu);

					CCLabelTTF* label = CCLabelTTF::create("cancel", "Arial",20);
					m_slotEquipMenu->addItem(label, "slotMenuCancel");

					CCLabelTTF* lEffect1 = CCLabelTTF::create("damage", "Helvetica", 20.0f);
					m_slotEquipMenu->addItem(lEffect1, "slotMenuModDamage");
					
					CCLabelTTF* lEffect2 = CCLabelTTF::create("heal", "Helvetica", 20.0f);
					m_slotEquipMenu->addItem(lEffect2, "slotMenuModHeal");
					return;
				}
			}

			for(int i=0; i< m_modSlots.size(); i++)
			{
				CCPoint sp = m_modSlots[i]->getPosition();

				if( p.getDistance(sp) <= slotRadius ) {
					CCLog("touched mod %d", i);

					m_slotEquipMenu = RadialLayer::create();
					m_effSelectedIdx = i;

					m_slotEquipMenu->setCenterNode(createPentNode(MOD_COLOR, ccc4f(0,0,0,1)));
					m_slotEquipMenu->setPosition( sp );
					addChild(m_slotEquipMenu);

					CCLabelTTF* label = CCLabelTTF::create("cancel", "Arial",20);
					m_slotEquipMenu->addItem(label, "slotMenuCancel");
					return;
				}
			}
		}
	}
}
