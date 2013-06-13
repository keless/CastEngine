#ifndef _PLAYERVIEW_H_
#define _PLAYERVIEW_H_

#include "GameEntityView.h"

class PlayerView :
	public GameEntityView
{
protected:
	void initView();

	CCNode* m_pParentView;

public:
	PlayerView(GameEntity* entity, CCNode* parent);
	~PlayerView(void);

	std::vector<GameAbilityView*>& getAbilityViews() { return m_abilityViews; }
};

#endif
