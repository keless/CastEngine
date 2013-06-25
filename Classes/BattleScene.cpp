#include "BattleScene.h"

USING_NS_CC;

#include "CastWorldModel.h"

#define GAME_UNIT_CONVERSION (1.0f/210.0f)
#define VIEW_UNIT_CONVERSION (210.0f)

CCScene* BattleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    BattleScene *layer = BattleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BattleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(BattleScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(pSprite, 0);

	m_battleMgr = BattleManager::create();
	addChild(m_battleMgr);

	setTouchEnabled(true);
	//registerWithTouchDispatcher();
    
    return true;
}

//virtual 
void BattleScene::update( float dt )
{

	
}



inline CCPoint locationInGLFromTouch(CCTouch& touch)
{
   auto director = CCDirector::sharedDirector();
   return director->convertToGL(touch.getLocationInView());
}


void BattleScene::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		//m_playerView->ccTouchBegan(touch, event);
	}
}

void BattleScene::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		//m_playerView->ccTouchMoved(touch, event);
	}
}

void BattleScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;
		/*
		m_playerView->ccTouchEnded(touch, event);
		if( m_playerModel->canCast() ) {
			std::vector<GameAbilityView*>& abilityViews = m_playerView->getAbilityViews();
			for( int i=0; i< abilityViews.size(); i++)
			{
				abilityViews[i]->ccTouchEnded(touch, event);
			}
		}

		int touchedIdx = -1;
		for( int i=0; i< m_enemies.size(); i++) 
		{
			if( m_enemies[i].enemyView->boundingBox().containsPoint( touch->getLocation() ) )
			{
				touchedIdx = i;
				break;
			}
		}

		if( touchedIdx != -1 ) {
			//dehighlight previous targets
			CastTarget* pTarget = m_playerModel->getTarget();
			for( int tIdx =0 ; tIdx< pTarget->getEntityList().size(); tIdx++) 
			{
				GameEntity* entity = (GameEntity*)pTarget->getEntityList()[tIdx];

				for( int vIdx=0; vIdx < m_enemies.size() ; vIdx++)
				{
					if( entity == m_enemies[vIdx].enemyModel )
					{
						GameEntityView* view = m_enemies[vIdx].enemyView;
						view->setHighlighted(false); 
						break;
					}
				}

			}
			pTarget->clearTargetEntities();
				
		
			//highlight new target
				
			pTarget->addTargetEntity( m_enemies[touchedIdx].enemyModel);
			m_enemies[touchedIdx].enemyView->setHighlighted(true);
			
		}
		*/

	}
}
void BattleScene::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
	for(auto it = touches->begin(); it != touches->end(); it++) 
	{
		auto touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		//m_playerView->ccTouchCancelled(touch, event);
	}
}


void BattleScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
