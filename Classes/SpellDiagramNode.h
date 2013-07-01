#ifndef _SPELLDIAGRAMNODE_H_
#define _SPELLDIAGRAMNODE_H_

#include "cocos2d.h"
using namespace cocos2d;

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
	SD_11_SCHOOL,
	SD_12_LEAF,
	SD_13_GREATER_PYRAMID,
	SD_14_GREATER_TRIQUETRA,

	SD_COUNT,

	SD_INVALID = -1
};

class SpellDiagramNode : public CCNode
{
	SpellDiagrams m_type;
	CCParticleSpiral* m_ps;

public:
	SpellDiagramNode(void);
	~SpellDiagramNode(void);
	CREATE_FUNC(SpellDiagramNode);

	bool init();

	void setDiagram( SpellDiagrams diagram );

	virtual void draw();
};

#endif
