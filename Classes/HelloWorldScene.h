#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "ZZProgressBar.h"
#include "GameEntity.h"
#include "GameEntityView.h"

#include "CastCommandState.h"
#include "CastCommandModel.h"

struct EntityPair
{
	GameEntity* enemyModel;
	GameEntityView* enemyView;
};

class HelloWorld : public cocos2d::CCLayer
{
	GameEntity* m_playerModel;
	GameEntityView* m_playerView;

	std::vector<EntityPair> m_enemies;

	std::map<std::string, CastCommandModel*> m_abilities;
	void initAbilities();
	void spawnEnemy();
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	virtual void update( float dt );

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    virtual void ccTouchesCancelled(CCSet* touches, CCEvent* event);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
