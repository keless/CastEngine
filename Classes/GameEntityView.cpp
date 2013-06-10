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
	int maxPerRow = 2;
	int row = 0;
	int col = 0;
	for( int i=0; i < abilities.size(); i++)
	{
		col = (i/maxPerRow);
		row = i - (col*maxPerRow);

		CastCommandState* ability = abilities[i];

		GameAbilityView* abilityView = new GameAbilityView( ability );
		m_abilityViews.push_back(abilityView);

		abilityView->setPositionX( col * abilityView->getContentSize().width );
		abilityView->setPositionY( row * abilityView->getContentSize().height );
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

void GameEntityView::setBackground( std::string imgName )
{
	CCSize size = this->getContentSize();
	CCTexture2D* tex =  CCTextureCache::sharedTextureCache()->addImage( imgName.c_str() );
	CCSprite* spriteBG = CCSprite::createWithTexture(tex);
	spriteBG->setScaleX( size.width / spriteBG->getContentSize().width );
	spriteBG->setScaleY( size.height / spriteBG->getContentSize().height );
	spriteBG->setAnchorPoint(ccp(0,0));
	addChild(spriteBG);
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

void GameEntityView::doLifedrain( ICastEntity* to )
{
	CCParticleFire* life = CCParticleFire::createWithTotalParticles(50);
	CCPoint point =  m_healthBar->boundingBox().origin;

	life->setStartColor( ccc4f(0,1.0f,0,1.0f) );
	life->setEndColor( ccc4f(0,1.0f,0,1.0f) );
	life->setPosition(ccp(0,0));

	//TODO: use physics interface to get position of 'to' entity and direct stream

	life->setGravity( CCPointMake( -90, 0 ) );
	life->setStartSize(25);
	life->setPosVar( CCPointMake( 10,10 ));

	m_healthBar->addChild(life);

	CCSequence* seq = CCSequence::create(
				CCDelayTime::create(0.5f),
				CCCallFunc::create(life, callfunc_selector(CCParticleFire::stopSystem)),
				CCDelayTime::create(2.5f),
				CCRemoveSelf::create(true),
				NULL
			);

	life->runAction(seq);
}

void GameEntityView::doHeal()
{
	CCParticleFire* heal = CCParticleFire::createWithTotalParticles(150);
	CCPoint point = CCPointMake(  m_healthBar->getContentSize().width/2, 0 );

	heal->setStartColor( ccc4f(1,1,1,1.0f) );
	heal->setEndColor( ccc4f(1,1,1,1.0f) );

	heal->setSpeed(20);
	heal->setStartSize(25);
	CCPoint posVar = heal->getPosVar();
	posVar.y /= 2;
	heal->setPosVar( posVar );

	heal->setPosition( point );
	m_healthBar->addChild(heal);

	CCSequence* seq = CCSequence::create(
					CCDelayTime::create(0.5f),
					CCCallFunc::create(heal, callfunc_selector(CCParticleFire::stopSystem)),
					CCDelayTime::create(2.5f),
					CCRemoveSelf::create(true),
					NULL
				);

	heal->runAction(seq);
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
		}else if( react.compare("lifedrain") == 0 ) {
			doLifedrain( evt->source->getOrigin() );
		}else if( react.compare("heal") == 0 ) {
			doHeal();
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

	//dont send touch if cant cast
	if( ! m_pEntity->canCast() ) return;

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