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
protected:
	ZZProgressBar* m_healthBar;
	ZZProgressBar* m_manaBar;
	ZZProgressBar* m_xpBar;
	GameEntity* m_pEntity;
	CCLabelTTF* m_lblName;
	CCLabelTTF* m_lblLevel;
	CCLayerColor* m_highlight;
	CCLayerColor* m_debuffIndicator;
	CCLayerColor* m_buffIndicator;

	std::vector<GameAbilityView*> m_abilityViews;

	virtual void initView();
	GameEntityView(){}

public:
	GameEntityView( GameEntity* entity );
	~GameEntityView(void);

	void detatchFromEntity();

	void setBackground( std::string imgName );

	void onStatUpdate(CCObject* e);
	void onShouldReact(CCObject* e);


	void setHighlighted( bool highlight );

	void doShake();
	void doBurn();
	void doLifedrain( ICastEntity* to );
	void doHeal();

	void updateView();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};


#endif
