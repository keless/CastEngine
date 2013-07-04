#ifndef _SPELLDIAGRAMNODE_H_
#define _SPELLDIAGRAMNODE_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "ZZUtils.h"
using namespace ZZ;

/*
custom node for rendering spell diagrams
*/

enum SpellDiagrams
{
	SD_01_NOVICE_CIRCLE = 0,
	SD_02_BLIND_EYE,
	SD_03_ADEPTS_CIRCLE,
	SD_04_LESSER_PYRAMID,
	SD_05_SERPENTS_EYE,
	SD_06_LESSER_TRIQUESTRA,
	SD_07_COMPASS,
	SD_08_FORTRESS,
	SD_09_DRAGONS_EYE,
	SD_10_SEEING_EYE,
	SD_11_LEAF,
	SD_12_GREATER_PYRAMID,
	SD_13_GREATER_TRIQUETRA,

	SD_COUNT,

	SD_INVALID = -1
};


class SpellDiagramNode : public CCLayer
{
	SpellDiagrams m_type;
	CCParticleSpiral* m_ps;

	RadialLayer* m_slotEquipMenu;

	float size;

	std::vector<CCDrawNode*> m_effectSlots;
	std::vector<CCDrawNode*> m_modSlots;

	void trimEffectsSize( int maxEffects );
	void trimModsSize( int maxMods );
	void addEffect( int idx, float x, float y, int level);
	void addMod( int idx, float x, float y, int level);

	void createModSlotMenu( RadialLayer* slotEquipMenu, CCPoint pos, int idx );
	void createEffSlotMenu( RadialLayer* slotEquipMenu, CCPoint pos, int idx );

	void onMenuCancel( CCObject* e );
	void onMenuMod(CCObject* e );
	void onMenuEff(CCObject* e );

public:
	SpellDiagramNode(void);
	~SpellDiagramNode(void);
	CREATE_FUNC(SpellDiagramNode);

	bool init();

	void setDiagram( SpellDiagrams diagram );

	void ccTouchesEnded(CCSet* touches, CCEvent* event);

	virtual void draw();
};

#endif
