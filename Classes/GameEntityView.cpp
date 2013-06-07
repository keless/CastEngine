#include "GameEntityView.h"


GameEntityView::GameEntityView( GameEntity* entity )
{
	m_pEntity = entity;
	m_pEntity->retain();

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

	scheduleUpdate();
}


GameEntityView::~GameEntityView(void)
{
	m_pEntity->release();
}

void GameEntityView::update( float delta )
{
	if(m_pEntity->isDirty() ) {
		updateView();
	}
}

void GameEntityView::updateView()
{
	m_healthBar->setProgress(  (m_pEntity->getProperty("hp_curr")) / (float) (m_pEntity->getProperty("hp_base")) );
	m_pEntity->setDirty(false);
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