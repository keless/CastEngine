#ifndef _CCGAMEENTITYVIEW_H_
#define _CCGAMEENTITYVIEW_H_


#include "cocos2d.h"
using namespace cocos2d;

#include "GameEntity.h"
#include "ZZProgressBar.h"

#include "json.h"

#include "GameAbilityView.h"

class GameEntityView :
	public CCNode
{
	ZZProgressBar* m_healthBar;
	GameEntity* m_pEntity;
	CCLabelTTF* m_lblName;

	std::vector<GameAbilityView*> m_abilityViews;
public:
	GameEntityView( GameEntity* entity );
	~GameEntityView(void);

	void updateView();

	virtual void update( float delta );
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};


#endif
