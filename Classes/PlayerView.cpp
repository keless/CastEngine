#include "PlayerView.h"


PlayerView::PlayerView(GameEntity* entity, CCNode* parent)
{
	m_pParentView = parent;
	m_pEntity = entity;
	m_pEntity->retain();

	m_pEntity->addListener("react", this, callfuncO_selector(GameEntityView::onShouldReact) );
	m_pEntity->addListener("incProperty", this, callfuncO_selector(GameEntityView::onStatUpdate) );

	initView();

	scheduleUpdate();
}


PlayerView::~PlayerView(void)
{

	for( int i=0; i<m_abilityViews.size(); i++)
	{
		m_abilityViews[i]->removeFromParentAndCleanup(true);
		CC_SAFE_RELEASE(m_abilityViews[i]);
	}
	m_abilityViews.clear();
}


//virtual 
void PlayerView::initView()
{
	setContentSize( CCSizeMake( 200, 200 ));

	CCLayerColor* bg = CCLayerColor::create(ccc4(25,25,25,255), 200,200);
	bg->setAnchorPoint(ccp(0,0));
	addChild(bg);

	m_highlight = CCLayerColor::create(ccc4(200,200,20,255), 204,204);
	m_highlight->setPosition(ccp(-2,-2));
	m_highlight->setAnchorPoint(ccp(0,0));
	addChild(m_highlight);
	setHighlighted(false);

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

	m_manaBar = ZZProgressBar::create( CCRectMake( 0,0, 100, 10 ) );
	m_manaBar->setMargin(2);
	m_manaBar->setColor( ccc3(50,50,255), ccc3(50,50,50) );
	m_manaBar->setProgress( (m_pEntity->getProperty("mana_curr")) / (float) (m_pEntity->getProperty("mana_base")) );
	m_manaBar->setAnchorPoint(ccp(0,1.0f));
	m_manaBar->setPositionY( m_healthBar->getPositionY() - (m_healthBar->getContentSize().height + 5) );
	addChild(m_manaBar, 48);


	//TODO: attach abilities to scene instead
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
		m_pParentView->addChild(abilityView, 51);
	}
}