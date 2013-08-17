#ifndef _SPELLDIAGRAMNODE_H_
#define _SPELLDIAGRAMNODE_H_

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
	int m_type;

	RadialLayer* m_slotEquipMenu;

	Json::Value m_spellDiagrams;

	Json::Value m_spellParts_Effects;
	Json::Value m_spellParts_Mods;

	float size;

	std::vector<CCDrawNode*> m_effectSlots;
	std::vector<CCDrawNode*> m_modSlots;

	std::map<int, std::string> m_effectsJson;
	std::map<int, std::string> m_modsJson;

	void prepareDiagram( int numEffects, int numMods );
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

	Json::Value getSpellDiagramJson();

	bool init();

	void setDiagram( int diagram );

	void ccTouchesEnded(CCSet* touches, CCEvent* event);

	virtual void draw();
};

#endif
