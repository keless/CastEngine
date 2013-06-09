#include "GameEntityView.h"


GameEntityView::GameEntityView( GameEntity* entity )
{
	m_pEntity = entity;
	m_pEntity->retain();

	m_pEntity->addListener("react", this, callfuncO_selector(GameEntityView::onShouldReact) );
	m_pEntity->addListener("incProperty", this, callfuncO_selector(GameEntityView::onStatUpdate) );

	setContentSize( CCSizeMake( 200, 200 ));

	CCLayerColor* bg = CCLayerColor::create(ccc4(25,25,25,255), 200,200);
	bg->setAnchorPoint(ccp(0,0));
	addChild(bg);

	m_lblName = CCLabelTTF::create();
	m_lblName->initWithString( m_pEntity->getName().c_str(), "Arial", 24.0f  );
	m_lblName->setAnchorPoint(ccp(0.0f, 1.0f ) );
	m_lblName->setPositionY(200);
	addChild(m_lblName, 50);

	m_healthBar = ZZProgressBar::create( CCRectMake(0,0, 100, 30) );
	m_healthBar->setMargin(2);
	m_healthBar->setProgress(  (m_pEntity->getProperty("hp_curr")) / (float) (m_pEntity->getProperty("hp_base")) );
	m_healthBar->setAnchorPoint(ccp(0.0f, 1.0f) );
	m_healthBar->setPositionY( 200 - m_lblName->getContentSize().height );
	addChild(m_healthBar, 49);

	std::vector<CastCommandState*>& abilities = m_pEntity->getAbilityList();
	for( int i=0; i < abilities.size(); i++)
	{
		CastCommandState* ability = abilities[i];

		GameAbilityView* abilityView = new GameAbilityView( ability );
		m_abilityViews.push_back(abilityView);

		abilityView->setPositionY( i * abilityView->getContentSize().height );
		addChild(abilityView, 51);
	}

	/*
	CCParticleFire* fire = CCParticleFire::create();
	fire->setPosition(ccp(0,0));
	//fire->setAnchorPoint(ccp(0,0));
	addChild(fire);
	*/

	scheduleUpdate();
}


GameEntityView::~GameEntityView(void)
{
	m_pEntity->remListener("react", this, callfuncO_selector(GameEntityView::onShouldReact) );
	m_pEntity->remListener("incProperty", this, callfuncO_selector(GameEntityView::onStatUpdate) );

	m_pEntity->release();
}

void GameEntityView::update( float delta )
{

}

void GameEntityView::onStatUpdate(CCObject* e)
{
	updateView();
}

void GameEntityView::doShake()
{
	CCLog("shake entity view ");
#define SHAKE_TAG 0xDEADBEEF
	if( getActionByTag(SHAKE_TAG) == NULL ) {
		CCSequence* shake = CCSequence::create(
			CCRotateBy::create(0.1f, 10, 0),
			CCRotateBy::create(0.2f, -20, 0),
			CCRotateBy::create(0.1f, 10, 0), 
			NULL);
		shake->setTag(SHAKE_TAG);

		runAction(shake);
	}
}

void GameEntityView::doBurn()
{
	CCLog("todo: particle system burn");

	CCParticleFire* fire = CCParticleFire::createWithTotalParticles(150);
	CCPoint point = CCPointMake(  m_healthBar->getContentSize().width/2, 0 );

	//fire->stopSystem();
	fire->setSpeed(20);
	fire->setStartSize(25);
	CCPoint posVar = fire->getPosVar();
	posVar.y /= 2;
	fire->setPosVar( posVar );

	fire->setPosition( point );
	m_healthBar->addChild(fire);

	CCSequence* seq = CCSequence::create(
					CCDelayTime::create(0.5f),
					CCCallFunc::create(fire, callfunc_selector(CCParticleFire::stopSystem)),
					CCDelayTime::create(2.5f),
					CCRemoveSelf::create(true),
					NULL
				);

	fire->runAction(seq);
}

void GameEntityView::onShouldReact(CCObject* e)
{
	GameEntityReactEvt* evt = dynamic_cast<GameEntityReactEvt*>(e);
	if( evt == NULL ) return;

	CCLog("todo: react to effect");

	if( evt->react.isString() ) {
		std::string react = evt->react.asString();
		if( react.compare("shake") == 0 ) {
			doShake();

		}else if( react.compare("burn") == 0 ) {
			doBurn();
		}
	}
}

void GameEntityView::updateView()
{
	m_healthBar->setProgress(  (m_pEntity->getProperty("hp_curr")) / (float) (m_pEntity->getProperty("hp_base")) );

}

bool GameEntityView::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	return boundingBox().containsPoint( touch->getLocationInView() );
}

void GameEntityView::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	for( int i=0; i< m_abilityViews.size(); i++) {
		m_abilityViews[i]->ccTouchMoved(touch, event);
	}
}

void GameEntityView::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint p = touch->getLocation();

	if( !boundingBox().containsPoint( p ) ) return;

	CCLog("game entity %s touched", m_pEntity->getName().c_str() );

	p.x -= getPositionX();
	p.y -= getPositionY();

	//activate ability if possible

	//todo: see if we can activate this ability or not (cant cast multiple abilities at the same time!)

	//dont send touch if any ability is casting
	std::vector<CastCommandState*>& abilityList = m_pEntity->getAbilityList();
	for( int i=0; i< abilityList.size(); i++) {
		if( abilityList[i]->isCasting() ) {
			//abort touch
			return;
		}
	}

	for( int i=0; i< m_abilityViews.size(); i++) {
		GameAbilityView* view = m_abilityViews[i];

		CCRect btn = view->boundingBox();
		//btn.origin.x += 
		
		if( !view->boundingBox().containsPoint( p ) ) continue;

		if( view->canCast() ) {
			view->startCast();
		}
		return;
	}


}
void GameEntityView::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{

}