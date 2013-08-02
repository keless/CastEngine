#include "PartyMemberEditor.h"


PartyMemberEditor::PartyMemberEditor(void)
{
	m_itemMenu = NULL;

	EventBus::get("pme")->addListener("zzrgrTrigger", this, callfuncO_selector(PartyMemberEditor::onTabSelect));
	EventBus::get("pme")->addListener("zztfEnd", this, callfuncO_selector(PartyMemberEditor::onPMNameChange));

	EventBus::game()->addListener("itmViewArmor", this, callfuncO_selector(PartyMemberEditor::onItemViewArmor));
	EventBus::game()->addListener("itmViewEquip", this, callfuncO_selector(PartyMemberEditor::onItemViewEquip));
	EventBus::game()->addListener("itmViewWeapon", this, callfuncO_selector(PartyMemberEditor::onItemViewWeap));

	EventBus::game()->addListener("itemMenuCancel", this, callfuncO_selector(PartyMemberEditor::onMenuCancel));
	EventBus::game()->addListener("itemMenuArmor", this, callfuncO_selector(PartyMemberEditor::onMenuArmor));
	EventBus::game()->addListener("itemMenuEquip", this, callfuncO_selector(PartyMemberEditor::onMenuEquip));
	EventBus::game()->addListener("itemMenuWeap", this, callfuncO_selector(PartyMemberEditor::onMenuWeap));
}


PartyMemberEditor::~PartyMemberEditor(void)
{
	EventBus::get("pme")->remListener("zzrgrTrigger", this, callfuncO_selector(PartyMemberEditor::onTabSelect));
	EventBus::get("pme")->remListener("zztfEnd", this, callfuncO_selector(PartyMemberEditor::onPMNameChange));

	EventBus::game()->remListener("itmViewArmor", this, callfuncO_selector(PartyMemberEditor::onItemViewArmor));
	EventBus::game()->remListener("itmViewEquip", this, callfuncO_selector(PartyMemberEditor::onItemViewEquip));
	EventBus::game()->remListener("itmViewWeapon", this, callfuncO_selector(PartyMemberEditor::onItemViewWeap));

	EventBus::game()->remListener("itemMenuCancel", this, callfuncO_selector(PartyMemberEditor::onMenuCancel));
	EventBus::game()->remListener("itemMenuArmor", this, callfuncO_selector(PartyMemberEditor::onMenuArmor));
	EventBus::game()->remListener("itemMenuEquip", this, callfuncO_selector(PartyMemberEditor::onMenuEquip));
	EventBus::game()->remListener("itemMenuWeap", this, callfuncO_selector(PartyMemberEditor::onMenuWeap));

	m_pEntity->release();

	for( int i=0; i< m_partyInv.size(); i++)
	{
		CC_SAFE_RELEASE( m_partyInv[i] );
	}
}

//static 
PartyMemberEditor* PartyMemberEditor::create( GameEntity* entity, const CCSize& size )
{
	PartyMemberEditor* obj = new PartyMemberEditor();
	if(!obj->init(entity, size))
	{
		delete obj;
		return NULL;
	}
	obj->autorelease();
	return obj;
}

bool PartyMemberEditor::init( GameEntity* entity, const CCSize& size )
{
	ignoreAnchorPointForPosition(false);

	m_pEntity = entity;
	m_pEntity->retain();

	setContentSize(size);

	CCSize tabSize = CCSizeMake(200,40);
	BaseRadioGroupLayer* tabCharacter = new BaseRadioGroupLayer();
	tabCharacter->initRadioGroup("pme", 0);
	tabCharacter->setAnchorPoint(ccp(0,1));
	tabCharacter->setPositionY(size.height);
	CCLabelTTF* lblChar = CCLabelTTF::create("Character", "Arial", 24, tabSize, kCCTextAlignmentCenter);
	tabCharacter->addChild(lblChar);
	tabCharacter->setContentSize(lblChar->getContentSize());
	lblChar->setPosition( ccp( tabSize.width/2, tabSize.height/2 ));
	addChild(tabCharacter);

	BaseRadioGroupLayer* tabItems = new BaseRadioGroupLayer();
	tabItems->initRadioGroup("pme", 1);
	tabItems->setAnchorPoint(ccp(0,1));
	tabItems->setPositionX(tabCharacter->getContentSize().width);
	tabItems->setPositionY(size.height);
	CCLabelTTF* lblItems = CCLabelTTF::create("Items", "Arial", 24, tabSize, kCCTextAlignmentCenter);
	tabItems->addChild(lblItems);
	tabItems->setContentSize(lblItems->getContentSize());
	lblItems->setPosition( ccp( tabSize.width/2, tabSize.height/2 ));
	addChild(tabItems);

	CCSize editSize = CCSizeMake( size.width, size.height - tabSize.height);
	m_editChar = CCLayerColor::create(ccc4(50,50,75,175), editSize.width, editSize.height);
	m_editChar->ignoreAnchorPointForPosition(false);
	m_editChar->setAnchorPoint(ccp(0,0));
	addChild(m_editChar);

	initCharView();

	m_editItems = CCLayerColor::create(ccc4(50,75,50,175), editSize.width, editSize.height);
	m_editItems->ignoreAnchorPointForPosition(false);
	m_editItems->setAnchorPoint(ccp(0,0));
	addChild(m_editItems);


	initItemsView();

	initPartyInventory();

	tabCharacter->triggerGroup();

	/*
	CCLabelTTF* title = CCLabelTTF::create("more people will come if we tell them we have punch and pie", "Arial", 24, CCSizeMake(400,100), kCCTextAlignmentCenter);
	title->setAnchorPoint(ccp(0.5,1));
	title->setPosition(ccp(size.width/2, size.height - 10));
	addChild(title);
	//*/


	return true;
}

void PartyMemberEditor::initPartyInventory()
{
	if( !IsFile( FILE_PARTY_INVENTORY_JSON ) ) {
		CCLog("load default party inventory");
		m_partyInventory = ReadFileToJson(FILE_DEFAULT_PARTY_INVENTORY_JSON);
	}else {
		CCLog("load party inventory from file");
		m_partyInventory = ReadFileToJson(FILE_PARTY_INVENTORY_JSON);
	}

	for( int i=0; i<  m_partyInventory.size(); i++)
	{
		const Json::Value& item = m_partyInventory[i];
		GameItem* gi = new GameItem("");
		gi->initFromJson(item);

		m_partyInv.push_back(gi);
	}
}

void PartyMemberEditor::initCharView()
{
	const CCSize& editSize = m_editChar->getContentSize();

	TextField* tf = TextField::create(m_pEntity->getName());
	//CCTextFieldTTF* tf = CCTextFieldTTF::textFieldWithPlaceHolder(defaultTxt.c_str(), "Arial", 28);
	tf->setAnchorPoint(ccp(0.5,1));
	tf->setPosition(ccp(editSize.width/2, editSize.height - 50));
	tf->setEventBus("pme");
	m_editChar->addChild(tf);

	CCNode* lblStr = CreateSimpleNamedLabel("str", "10");
	lblStr->setPosition(50, editSize.height - 100 );
	m_editChar->addChild(lblStr);

	CCNode* lblInt = CreateSimpleNamedLabel("int", "10");
	lblInt->setPosition(50, editSize.height - 150 );
	m_editChar->addChild(lblInt);

	CCNode* lblAgi = CreateSimpleNamedLabel("dex", "10");
	lblAgi->setPosition(50, editSize.height - 200 );
	m_editChar->addChild(lblAgi);

}

void PartyMemberEditor::initItemsView()
{
	const CCSize& editSize = m_editChar->getContentSize();

	CCLabelTTF* lbl = CCLabelTTF::create("Armor", "Arial", 24.0f);
	lbl->setAnchorPoint(ccp(0.5,0.5));
	lbl->setPositionX( editSize.width/2 );
	lbl->setPositionY( editSize.height * 0.75f);

	m_editItems->addChild(lbl);

	m_itmView[GIT_ARMOR] = new GameItemView();
	
	TouchableNode* tn = new TouchableNode("itmViewArmor");
	tn->setPosition( ccp( editSize.width/2, editSize.height * 0.75f ) );
	tn->addChild(m_itmView[GIT_ARMOR]);

	m_editItems->addChild(tn);


	lbl = CCLabelTTF::create("Equipment", "Arial", 24.0f);
	lbl->setAnchorPoint(ccp(0.5,0.5));
	lbl->setPositionX( editSize.width * 0.33f );
	lbl->setPositionY( editSize.height * 0.33f);
	m_editItems->addChild(lbl);

	m_itmView[GIT_EQUIPMENT] = new GameItemView();

	tn = new TouchableNode("itmViewEquip");
	tn->setPosition( ccp( editSize.width * 0.33f, editSize.height * 0.33f ) );
	tn->addChild(m_itmView[GIT_EQUIPMENT]);

	m_editItems->addChild(tn);


	lbl = CCLabelTTF::create("Weapon", "Arial", 24.0f);
	lbl->setAnchorPoint(ccp(0.5,0.5));
	lbl->setPositionX( editSize.width * 0.66f );
	lbl->setPositionY( editSize.height * 0.33f);
	m_editItems->addChild(lbl);

	m_itmView[GIT_WEAPON] = new GameItemView();

	tn = new TouchableNode("itmViewWeapon");
	tn->setPosition( ccp( editSize.width * 0.66f, editSize.height * 0.33f ) );
	tn->addChild(m_itmView[GIT_WEAPON]);

	m_editItems->addChild(tn);

}

void PartyMemberEditor::clearItemMenu()
{
	if( m_itemMenu != NULL ) 
	{
		m_itemMenu->removeFromParentAndCleanup(true);
		CC_SAFE_RELEASE_NULL(m_itemMenu);
	}
}

void PartyMemberEditor::resetItemMenu()
{
	clearItemMenu();

	m_itemMenu = RadialLayer::create();
	m_itemMenu->retain();
	//menu->setCenterNode();
	addChild(m_itemMenu);

	CCLabelTTF* label = CCLabelTTF::create("cancel", "Arial",20);
	m_itemMenu->addItem(label, "itemMenuCancel");
}

void PartyMemberEditor::onItemViewArmor(CCObject* e)
{
	resetItemMenu();
	m_itemMenu->setPosition( m_itmView[ GIT_ARMOR ]->getParent()->getPosition() );

	for( int i=0; i< m_partyInv.size(); i++){
		if( m_partyInv[i]->getType() == GIT_ARMOR ) 
		{
			std::string name = m_partyInv[i]->getName();
			Json::Value data;
			data["name"] = name;
			m_itemMenu->addItemWithJson( CreateSimpleLabel( name ), "itemMenuArmor",  data);
		}
	}

}
void PartyMemberEditor::onItemViewEquip(CCObject* e)
{
	resetItemMenu();
	m_itemMenu->setPosition( m_itmView[ GIT_EQUIPMENT ]->getParent()->getPosition() );


	for( int i=0; i< m_partyInv.size(); i++){
		if( m_partyInv[i]->getType() == GIT_EQUIPMENT ) 
		{
			std::string name = m_partyInv[i]->getName();
			Json::Value data;
			data["name"] = name;
			m_itemMenu->addItemWithJson( CreateSimpleLabel( name ), "itemMenuEquip",  data);
		}
	}
}
void PartyMemberEditor::onItemViewWeap(CCObject* e)
{
	resetItemMenu();
	m_itemMenu->setPosition( m_itmView[ GIT_WEAPON ]->getParent()->getPosition() );

	for( int i=0; i< m_partyInv.size(); i++){
		if( m_partyInv[i]->getType() == GIT_WEAPON ) 
		{
			std::string name = m_partyInv[i]->getName();
			Json::Value data;
			data["name"] = name;
			m_itemMenu->addItemWithJson( CreateSimpleLabel( name ), "itemMenuWeap",  data);
		}
	}
}

void PartyMemberEditor::onMenuCancel( CCObject* e )
{
	clearItemMenu();
}

void PartyMemberEditor::onMenuArmor( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	std::string name = evt->json["name"].asString();
	CCLog( name.c_str() );

	clearItemMenu();
}
void PartyMemberEditor::onMenuWeap( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	std::string name = evt->json["name"].asString();
	CCLog( name.c_str() );

	clearItemMenu();
}
void PartyMemberEditor::onMenuEquip( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	std::string name = evt->json["name"].asString();
	CCLog( name.c_str() );

	clearItemMenu();
}

void PartyMemberEditor::onTabSelect( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	int tabIdx = evt->json["index"].asInt();

	CCLog("todo: member tab select %d", tabIdx);

	switch(tabIdx) {
	case 0:
		m_editChar->setVisible(true);
		m_editItems->setVisible(false);
		break;
	case 1:
		m_editChar->setVisible(false);
		m_editItems->setVisible(true);
		break;
	}

}

void PartyMemberEditor::onPMNameChange( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	TextField* tf = (TextField*) evt->pUserData;

	std::string name = tf->getString();

	if( name.size() > 2 ) {
		m_pEntity->setName( name );

		JsonEvent* jsonEvent = new JsonEvent("partyMemberEdited");
		jsonEvent->json["pGameEntity"] = m_pEntity;
		EventBus::game()->dispatch(jsonEvent);
	}
}