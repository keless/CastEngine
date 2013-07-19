#include "SpellDiagramNode.h"

#define DSIZE 400
#define TRANSITION_TIME 0.5f
#define MOD_COLOR ccc4f(0,1,0,1)
#define EFF_COLOR ccc4f(1,0,0,1)

SpellDiagramNode::SpellDiagramNode(void)
{
	m_type = SD_INVALID;
	m_slotEquipMenu = NULL; 
	
}


SpellDiagramNode::~SpellDiagramNode(void)
{
	EventBus::game()->remListener("slotMenuCancel", this, callfuncO_selector(SpellDiagramNode::onMenuCancel));
	EventBus::game()->remListener("slotMenuMod", this, callfuncO_selector(SpellDiagramNode::onMenuMod));
	EventBus::game()->remListener("slotMenuEff", this, callfuncO_selector(SpellDiagramNode::onMenuEff));
}

bool SpellDiagramNode::init()
{
	setAnchorPoint(ccp(0.5f,0.5f));
	ignoreAnchorPointForPosition(false);
	//setContentSize(CCSizeMake(100,100));
	size = DSIZE;

	m_spellParts = ReadFileToJson( "spellParts.json" );

	m_spellDiagrams = ReadFileToJson("spellDiagrams.json");
	if( m_spellDiagrams.isMember("diagrams") ) {
		m_spellDiagrams = m_spellDiagrams["diagrams"];
	}

	EventBus::game()->addListener("slotMenuCancel", this, callfuncO_selector(SpellDiagramNode::onMenuCancel));
	EventBus::game()->addListener("slotMenuMod", this, callfuncO_selector(SpellDiagramNode::onMenuMod));
	EventBus::game()->addListener("slotMenuEff", this, callfuncO_selector(SpellDiagramNode::onMenuEff));


	setTouchEnabled(true);

	return true;
}

/***********
{
 "diagramLevel":1,
 "diagramName":"novice circle",
 "mods":[null, "glyph of something" ,null],
 "effects":["effect of derp", null]
}
************/
Json::Value SpellDiagramNode::getSpellDiagramJson()
{
	Json::Value json;

	Json::Value& diagram = m_spellDiagrams[m_type];
	json["diagramLevel"] = diagram["lines"].size();
	json["diagramName"] = diagram["name"];

	/*
	switch( m_type ) {
	case SD_01_NOVICE_CIRCLE:
		json["diagramLevel"] = 1;
		json["diagramName"] = "Novice Circle";
		break;
	case SD_02_BLIND_EYE:
		json["diagramLevel"] = 2;
		json["diagramName"] = "Blind Eye";
		break;
	case SD_03_ADEPTS_CIRCLE:
		json["diagramLevel"] = 2;
		json["diagramName"] = "Adept's Circle";
		break;

	case SD_04_LESSER_PYRAMID:
		json["diagramLevel"] = 3;
		json["diagramName"] = "Lesser Pyramid";
		break;

	case SD_05_SERPENTS_EYE:
		json["diagramLevel"] = 3;
		json["diagramName"] = "Serpent's Eye";
		break;

	case SD_06_LESSER_TRIQUESTRA:
		json["diagramLevel"] = 3;
		json["diagramName"] = "Lesser Triquestra";
		break;

	case SD_07_COMPASS:
		json["diagramLevel"] = 3;
		json["diagramName"] = "Compass";
		break;

	case SD_08_FORTRESS:
		json["diagramLevel"] = 4;
		json["diagramName"] = "Fortress";
		break;

	case SD_09_DRAGONS_EYE:
		json["diagramLevel"] = 4;
		json["diagramName"] = "Dragon's Eye";
		break;

	case SD_10_SEEING_EYE:
		json["diagramLevel"] = 4;
		json["diagramName"] = "Seeing Eye";
		break;

	case SD_11_LEAF:
		json["diagramLevel"] = 4;
		json["diagramName"] = "Leaf";
		break;
	
	case SD_12_GREATER_PYRAMID:
		json["diagramLevel"] = 4;
		json["diagramName"] = "Greater Pyramid";
		break;

	case SD_13_GREATER_TRIQUETRA:
		json["diagramLevel"] = 4;
		json["diagramName"] = "Greater Triquetra";
		break;
	}
	*/

	json["effects"] = Json::Value();

	for( int i=0; i< m_effectSlots.size(); i++)
	{
		if( m_effectsJson.count(i) == 0 )
		{
			json["effects"].append( Json::Value() );
		}else {
			json["effects"].append( m_effectsJson[i] );
		}
	}

	json["mods"] = Json::Value();

	for( int i=0; i< m_modSlots.size(); i++)
	{
		if( m_modsJson.count(i) == 0 )
		{
			json["mods"].append( Json::Value() );
		}else {
			json["mods"].append( m_modsJson[i] );
		}
	}

	return json;
}

CCDrawNode* createPentNode( ccColor4F fill, ccColor4F outline)
{
	CCPoint pent[5];
	pent[0] = CCPointMake(0,10);
	pent[1] = CCPointMake(10,2);
	pent[2] = CCPointMake(5,-7);
	pent[3] = CCPointMake(-5,-7);
	pent[4] = CCPointMake(-10,2);

	CCDrawNode* pt = CCDrawNode::create();
	pt->drawPolygon(pent, 5, fill, 1, outline);
	return pt;
}

void SpellDiagramNode::createModSlotMenu( RadialLayer* slotEquipMenu, CCPoint pos, int idx )
{
	
		m_slotEquipMenu->setCenterNode(createPentNode(MOD_COLOR, ccc4f(0,0,0,1)));
		m_slotEquipMenu->setPosition( pos );
		addChild(m_slotEquipMenu);

		CCLabelTTF* label = CCLabelTTF::create("cancel", "Arial",20);
		m_slotEquipMenu->addItem(label, "slotMenuCancel");

		Json::Value mods = m_spellParts.get("mods", Json::Value());
		Json::Value::Members modNames = mods.getMemberNames();
		for( int i=0; i<modNames.size(); i++)
		{
			label = CCLabelTTF::create(modNames[i].c_str(), "Helvetica", 20.0f);
			Json::Value data;
			data["name"] = modNames[i];
			data["idx"] = idx;
			m_slotEquipMenu->addItemWithJson(label, "slotMenuMod", data);
		}

}
void SpellDiagramNode::createEffSlotMenu( RadialLayer* slotEquipMenu, CCPoint pos, int idx )
{
		m_slotEquipMenu->setCenterNode(createPentNode(EFF_COLOR, ccc4f(0,0,0,1)));
		m_slotEquipMenu->setPosition( pos );
		addChild(m_slotEquipMenu);

		CCLabelTTF* label = CCLabelTTF::create("cancel", "Arial",20);
		m_slotEquipMenu->addItem(label, "slotMenuCancel");


		Json::Value mods = m_spellParts.get("effects", Json::Value());
		Json::Value::Members modNames = mods.getMemberNames();
		for( int i=0; i<modNames.size(); i++)
		{
			label = CCLabelTTF::create(modNames[i].c_str(), "Helvetica", 20.0f);
			Json::Value data;
			data["name"] = modNames[i];
			data["idx"] = idx;
			m_slotEquipMenu->addItemWithJson(label, "slotMenuEff", data);
		}

}

void SpellDiagramNode::onMenuCancel( CCObject* e )
{
	if( m_slotEquipMenu != NULL ) {
		m_slotEquipMenu->removeFromParentAndCleanup(true);
		m_slotEquipMenu = NULL;
	}
}
void SpellDiagramNode::onMenuMod(CCObject* e )
{
	
	if( m_slotEquipMenu != NULL ) {
		JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
		if(evt == NULL ) return;

		std::string modName = evt->json.get("name", "").asString();
		int modIdx = evt->json.get("idx", -1).asInt();

		Json::Value mods = m_spellParts.get("mods", Json::Value());
		if( mods.isMember(modName) ) {
			//Json::Value& sel = mods[modName];
			
			//handle mod selection
			CCLog("todo: apply mod %s to idx %d", modName.c_str(), modIdx);

			//clear any previous labels
			m_modSlots[modIdx]->removeAllChildrenWithCleanup(true);

			//store json
			m_modsJson[ modIdx ] = modName; //sel;

			//apply label
			CCLabelTTF* label = CCLabelTTF::create(modName.c_str(), "Helevetica", 12, CCSizeMake(size/5, size/5), kCCTextAlignmentCenter);
			label->setAnchorPoint(ccp(0.5f,0.5f));
			m_modSlots[modIdx]->addChild(label);

		}

		//clean up menu
		m_slotEquipMenu->removeFromParentAndCleanup(true);
		m_slotEquipMenu = NULL;

		//send update event
		JsonEvent* updateEvt = new JsonEvent("spellEditorUpdate");
		updateEvt->json = getSpellDiagramJson();
		EventBus::game()->dispatch(updateEvt);
	}
}
void SpellDiagramNode::onMenuEff(CCObject* e )
{
	if( m_slotEquipMenu != NULL  ) {
		JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
		if(evt == NULL ) return;

		std::string modName = evt->json.get("name", "").asString();
		int modIdx = evt->json.get("idx", -1).asInt();


		Json::Value mods = m_spellParts.get("effects", Json::Value());
		if( mods.isMember(modName) ) {
			//Json::Value& sel = mods[modName];
			

			//handle mod selection
			CCLog("todo: apply mod %s to idx %d", modName.c_str(), modIdx);

			//clear any previous labels
			m_effectSlots[modIdx]->removeAllChildrenWithCleanup(true);

			//store json
			m_effectsJson[ modIdx ] = modName; //sel;

			//apply label
			CCLabelTTF* label = CCLabelTTF::create(modName.c_str(), "Helevetica", 12, CCSizeMake(size/5, size/5), kCCTextAlignmentCenter);
			label->setAnchorPoint(ccp(0.5f,0.5f));
			m_effectSlots[modIdx]->addChild(label);

		}

		//handle eff selection
		CCLog("todo: apply eff %s to idx %d", modName.c_str(), modIdx);

		//clean up menu
		m_slotEquipMenu->removeFromParentAndCleanup(true);
		m_slotEquipMenu = NULL;

		//send update event
		JsonEvent* updateEvt = new JsonEvent("spellEditorUpdate");
		updateEvt->json = getSpellDiagramJson();
		EventBus::game()->dispatch(updateEvt);
	}
}



void SpellDiagramNode::addEffect( int idx, float x, float y, int level ) {


	if( idx < m_effectSlots.size() )
	{
		CCDrawNode* pt = m_effectSlots[idx];
		pt->runAction( CCMoveTo::create(TRANSITION_TIME, ccp(x,y)) );
	}else {
		//create new
		CCDrawNode* pt = createPentNode(EFF_COLOR, ccc4f(0,0,0,1));
		pt->setPosition(x,y);
		addChild(pt);
		pt->setScale(0.01);
		pt->runAction(CCScaleTo::create(TRANSITION_TIME/2, 1,1));
		m_effectSlots.push_back(pt);
	}
}



void SpellDiagramNode::addMod( int idx, float x, float y, int level ) {

	if( idx < m_modSlots.size() )
	{
		CCDrawNode* pt = m_modSlots[idx];
		pt->runAction( CCMoveTo::create(TRANSITION_TIME, ccp(x,y)) );
	}else {

		CCDrawNode* pt = createPentNode(MOD_COLOR, ccc4f(0,0,0,1));
		pt->setPosition(x,y);
		addChild(pt);
		pt->setScale(0.01);
		pt->runAction(CCScaleTo::create(TRANSITION_TIME/2, 1,1));
		m_modSlots.push_back(pt);
	}
}

void SpellDiagramNode::prepareDiagram( int numEffects, int numMods )
{
	m_effectsJson.clear();
	m_modsJson.clear();

	for( int i=0; i< m_effectSlots.size();i++)
	{
		m_effectSlots[i]->removeAllChildrenWithCleanup(true);
	}
	for( int i=0; i< m_modSlots.size();i++)
	{
		m_modSlots[i]->removeAllChildrenWithCleanup(true);
	}

	trimEffectsSize(numEffects);
	trimModsSize(numMods);
}

void SpellDiagramNode::trimEffectsSize( int maxEffects )
{
	if( m_effectSlots.size() <= maxEffects ) return;

	for( int i= maxEffects; i < m_effectSlots.size(); i++ ) {
		m_effectSlots[i]->removeFromParentAndCleanup(true);
	}
	m_effectSlots.erase( m_effectSlots.begin() + maxEffects, m_effectSlots.end() );

}
void SpellDiagramNode::trimModsSize( int maxMods )
{
	
	if( m_modSlots.size() <= maxMods ) return;

	for( int i= maxMods; i < m_modSlots.size(); i++ ) {
		m_modSlots[i]->removeFromParentAndCleanup(true);
	}
	m_modSlots.erase( m_modSlots.begin() + maxMods, m_modSlots.end() );
}

//triangle
CCPoint tA = ccp(0, DSIZE*0.5); //top
CCPoint tB = ccp(DSIZE*0.4330, -DSIZE*0.25); //br
CCPoint tC = ccp(-DSIZE*0.4330, -DSIZE*0.25); //bl
//serpents eye
CCPoint seT = ccp(0, DSIZE*0.38);
CCPoint seB = ccp(0, -DSIZE*0.38);
//leaf
CCPoint leT = ccp(0, DSIZE*0.5);
CCPoint leB = ccp(0, -DSIZE*0.15);

void SpellDiagramNode::setDiagram( int diagram )
{
	if( diagram >= m_spellDiagrams.size() )
		return; //out of bounds

	m_type = diagram;

	Json::Value& spellDiagram = m_spellDiagrams[m_type];
	Json::Value& effects = spellDiagram["effects"];
	Json::Value& mods = spellDiagram["mods"];

	prepareDiagram(effects.size(), mods.size());
	for( int i=0; i< effects.size(); i++)
	{
		float level = effects[i]["level"].asDouble();
		float x = effects[i]["x"].asDouble();
		float y = effects[i]["y"].asDouble();
		addEffect(i, size*x,size*y, level);
	}
	for( int i=0; i< mods.size(); i++)
	{
		float level = mods[i]["level"].asDouble();
		float x = mods[i]["x"].asDouble();
		float y = mods[i]["y"].asDouble();
		addMod(i, size*x,size*y, level);
	}


	/*
	CCDrawNode* pt = NULL;
	switch( m_type ) {


	case SD_05_SERPENTS_EYE:
		prepareDiagram(2,4);
		addEffect(0, -size*0.25,0, 3);
		addEffect(1,  size*0.25,0, 3);
		addMod(0, seT.x,seT.y, 3);
		addMod(1, seB.x,seB.y, 3);
		addMod(2, -size*0.5,0, 2);
		addMod(3,  size*0.5,0, 2);
		break;

	case SD_06_LESSER_TRIQUESTRA:
		prepareDiagram(4,6);
		addEffect(0, 0,size*0.02, 3);
		addEffect(1, 0,size*0.25, 3);
		addEffect(2, -size*0.25,-size*0.1, 3);
		addEffect(3,  size*0.25,-size*0.1, 3);
		addMod(0, -size*0.1667,size*0.1, 4);
		addMod(1,  size*0.1667,size*0.1, 4);
		addMod(2,  0,-size*0.15, 4);
		addMod(3, tA.x,tA.y, 2);
		addMod(4, tB.x,tB.y, 2);
		addMod(5, tC.x,tC.y, 2);
		break;

	case SD_07_COMPASS:
		prepareDiagram(4,5);
		addEffect(0, -size*0.25, size*0.25, 3);
		addEffect(1,  size*0.25, size*0.25, 3);
		addEffect(2,  size*0.25,-size*0.25, 3);
		addEffect(3, -size*0.25,-size*0.25, 3);

		addMod(0, 0,0, 4);
		addMod(1, 0, size*0.5, 3);
		addMod(2, size*0.5,0, 3);
		addMod(3, 0,-size*0.5, 3);
		addMod(4, -size*0.5,0, 3);
		break;

	case SD_08_FORTRESS:
		prepareDiagram(1,4);
		addEffect(0, 0,0, 4);
		addMod(0, -size*0.5, size*0.5, 2);
		addMod(1,  size*0.5, size*0.5, 2);
		addMod(2,  size*0.5,-size*0.5, 2);
		addMod(3, -size*0.5,-size*0.5, 2);
		break;

	case SD_09_DRAGONS_EYE:
		prepareDiagram(3,4);
		addEffect(0, 0,0, 2);
		addEffect(1, -size*0.375,0, 2);
		addEffect(2,  size*0.375,0, 2);

		addMod(0, seT.x,seT.y, 4);
		addMod(1, seB.x,seB.y, 4);
		addMod(2, -size*0.5,0, 2);
		addMod(3,  size*0.5,0, 2);
		break;

	case SD_10_SEEING_EYE:
		prepareDiagram(3,2);
		addEffect(0, 0,0, 2);
		addEffect(1, 0, size*0.375, 2);
		addEffect(2, 0,-size*0.375, 2);


		addMod(0, -size*0.5,0, 2);
		addMod(1,  size*0.5,0, 2);
		break;

	case SD_11_LEAF:
		prepareDiagram(6,7);
		addEffect(0, -size*0.075,size*0.02, 3);
		addEffect(1, -size*0.075,size*0.25, 3);
		addEffect(2,  size*0.075,size*0.02, 3);
		addEffect(3,  size*0.075,size*0.25, 3);
		addEffect(4, -size*0.25,-size*0.1, 3);
		addEffect(5,  size*0.25,-size*0.1, 3);
		addMod(0, -size*0.1667,size*0.1, 4);
		addMod(1,  size*0.1667,size*0.1, 4);
		addMod(2, 0,-size*0.15, 4);
		addMod(3, 0,size*0.15, 2);
		addMod(4, tA.x,tA.y, 2);
		addMod(5, tB.x,tB.y, 2);
		addMod(6, tC.x,tC.y, 2);

		break;
	
	case SD_12_GREATER_PYRAMID:
		prepareDiagram(4,3);
		addEffect(0, 0,0, 3);
		addEffect(1, -size*0.3,size*0.20, 3);
		addEffect(2,  size*0.3,size*0.20, 3);
		addEffect(3,  0,-size*0.35, 3);
		addMod(0, tA.x,tA.y, 4);
		addMod(1, tB.x,tB.y, 4);
		addMod(2, tC.x,tC.y, 4);
		break;

	case SD_13_GREATER_TRIQUETRA:
		prepareDiagram(7,6);
		addEffect(0, 0,size*0.02, 3);
		addEffect(1, 0,size*0.25, 3);
		addEffect(2, -size*0.25,-size*0.1, 3);
		addEffect(3,  size*0.25,-size*0.1, 3);


		addEffect(4, -size*0.3,size*0.20, 3);
		addEffect(5,  size*0.3,size*0.20, 3);
		addEffect(6,  0,-size*0.35, 3);

		addMod(0, -size*0.1667,size*0.1, 4);
		addMod(1,  size*0.1667,size*0.1, 4);
		addMod(2,  0,-size*0.15, 4);
		addMod(3, tA.x,tA.y, 4);
		addMod(4, tB.x,tB.y, 4);
		addMod(5, tC.x,tC.y, 4);
		break;

	default:
		prepareDiagram(0,0);
	}
	*/

	JsonEvent* evt = new JsonEvent("spellEditorUpdate");
	evt->json = getSpellDiagramJson();
	EventBus::game()->dispatch("spellEditorUpdate", evt);

}

//virtual 
void SpellDiagramNode::draw()
{
	CCNode::draw();

	float ptSz = 5;

	ccDrawInit();
	ccPointSize(ptSz);
	glLineWidth(ptSz);
	//ccDrawColor4B(0, 0, 256, 256);
	ccDrawColor4F(0.0f, 0.0f, 1.0f, 1.0f);
    //ccDrawLine(ccp(0,0), ccp(100, 100));

	Json::Value& lines = m_spellDiagrams[m_type]["lines"];
	for( int i=0; i< lines.size(); i++)
	{
		std::string type = lines[i].get("type", "circle").asString();
		if( type.compare("circle") == 0 ) {
			ccDrawCircle(ccp(0,0), size/2, 0, 32, false);
		}else if( type.compare("bezier") == 0 ) {
			Json::Value& pts = lines[i]["points"];
			ccDrawCubicBezier(ccp(size*pts[0u].asDouble(), size*pts[1].asDouble()), 
							ccp(size*pts[2].asDouble(), size*pts[3].asDouble()),
							ccp(size*pts[4].asDouble(), size*pts[5].asDouble()),
							ccp(size*pts[6].asDouble(), size*pts[7].asDouble()), 32);
		}else if( type.compare("line") == 0 ) {
			Json::Value& pts = lines[i]["points"];
			ccDrawLine(ccp(size*pts[0u].asDouble(), size*pts[1].asDouble()), 
							ccp(size*pts[2].asDouble(), size*pts[3].asDouble()) );
		}
	}


/*

	if( m_type == SD_08_FORTRESS )  {
		//square shape
		ccDrawLine(ccp(-size/2 - ptSz/2, size/2), ccp(size/2 + ptSz/2, size/2)); //top
		ccDrawLine(ccp(-size/2 - ptSz/2, -size/2), ccp(size/2 + ptSz/2, -size/2)); //btm

		ccDrawLine(ccp(size/2, size/2), ccp(size/2 , -size/2)); //right
		ccDrawLine(ccp(-size/2, size/2), ccp(-size/2 , -size/2)); //left
	}

	if( m_type == SD_06_LESSER_TRIQUESTRA || m_type == SD_11_LEAF || 
		m_type == SD_13_GREATER_TRIQUETRA ) 
	{
		float topY = tA.y;
		float botY = tB.y;
		float midY = 0;

		CCPoint lcp1 = ccp(size*0.1666, -size*0.2167);
		CCPoint lcp2 = ccp(size*0.3335, size*0.1233);
		ccDrawCubicBezier(tC, lcp1, lcp2, tA, 32); //left

		CCPoint rcp1 = ccp(-size*0.1666, -size*0.2167);
		CCPoint rcp2 = ccp(-size*0.3335, size*0.1233);
		ccDrawCubicBezier(tB, rcp1, rcp2, tA, 32); //right

		CCPoint bcp1 = ccp(-size*0.25, size*0.286);
		CCPoint bcp2 = ccp( size*0.25, size*0.286);
		ccDrawCubicBezier(tC, bcp1, bcp2, tB, 32); //bottom
	}

	if( m_type == SD_07_COMPASS || m_type == SD_03_ADEPTS_CIRCLE ) {
		ccDrawLine(ccp(0, size/2), ccp(0, -size/2)); 
	}

	if( m_type == SD_05_SERPENTS_EYE ) {
		//shorter verticle line for eye
		ccDrawLine( seT, seB);
	}

	if( m_type == SD_11_LEAF ) {
		//offset verticle line for leaf
		ccDrawLine(leT, leB);
	}

	if( m_type == SD_09_DRAGONS_EYE ) {
		//shorter eye
		CCPoint t = ccp(0, size*0.38);
		CCPoint b = ccp(0, -size*0.38);
		CCPoint rcp1 = ccp(size/3,size/5);
		CCPoint rcp2 = ccp(size/3,-size/5);
		ccDrawCubicBezier(t, rcp1, rcp2, b, 32);
		CCPoint lcp1 = ccp(-size/3,size/5);
		CCPoint lcp2 = ccp(-size/3,-size/5);
		ccDrawCubicBezier(t, lcp1, lcp2, b, 32);
	}

	if( m_type == SD_10_SEEING_EYE ) {
		//shorter eye
		ccDrawCubicBezier(ccp(-size/2, 0), ccp(-size/3, size/4), ccp(size/3,size/4), ccp(size/2,0), 32);
		ccDrawCubicBezier(ccp(-size/2, 0), ccp(-size/3, -size/4), ccp(size/3,-size/4), ccp(size/2,0), 32);
	}

	if( m_type == SD_07_COMPASS  ) {
		//horizontal line
		ccDrawLine(ccp(-size/2, 0), ccp(size/2, 0));
	}
	*/
}

void SpellDiagramNode::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	float slotRadius = 25.0f;

	for(CCSetIterator it = touches->begin(); it != touches->end(); it++) 
	{
		CCTouch* touch = dynamic_cast<CCTouch*>(*it);
		if(touch == NULL)
			break;

		if( m_slotEquipMenu == NULL ) {

			CCPoint p = touch->getLocation();
			p.x -= getPositionX();
			p.y -= getPositionY();

			for(int i=0; i< m_effectSlots.size(); i++)
			{
				CCPoint sp = m_effectSlots[i]->getPosition();

				if( p.getDistance(sp) <= slotRadius ) {
					CCLog("touched effect %d", i);

					m_slotEquipMenu = RadialLayer::create();
					createEffSlotMenu(m_slotEquipMenu, sp, i);
					

					return;
				}
			}

			for(int i=0; i< m_modSlots.size(); i++)
			{
				CCPoint sp = m_modSlots[i]->getPosition();

				if( p.getDistance(sp) <= slotRadius ) {
					CCLog("touched mod %d", i);

					m_slotEquipMenu = RadialLayer::create();
					createModSlotMenu(m_slotEquipMenu, sp, i);

					return;
				}
			}
		}
	}
}
