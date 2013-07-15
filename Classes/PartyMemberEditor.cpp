#include "PartyMemberEditor.h"


PartyMemberEditor::PartyMemberEditor(void)
{

	EventBus::get("pme")->addListener("zzrgrTrigger", this, callfuncO_selector(PartyMemberEditor::onTabSelect));
	EventBus::get("pme")->addListener("zztfEnd", this, callfuncO_selector(PartyMemberEditor::onPMNameChange));
}


PartyMemberEditor::~PartyMemberEditor(void)
{
	EventBus::get("pme")->remListener("zzrgrTrigger", this, callfuncO_selector(PartyMemberEditor::onTabSelect));
	EventBus::get("pme")->remListener("zztfEnd", this, callfuncO_selector(PartyMemberEditor::onPMNameChange));

	m_pEntity->release();
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
	tabItems->initRadioGroup("pme", 0);
	tabItems->setAnchorPoint(ccp(0,1));
	tabItems->setPositionX(tabCharacter->getContentSize().width);
	tabItems->setPositionY(size.height);
	CCLabelTTF* lblItems = CCLabelTTF::create("Items", "Arial", 24, tabSize, kCCTextAlignmentCenter);
	tabItems->addChild(lblItems);
	tabItems->setContentSize(lblItems->getContentSize());
	lblItems->setPosition( ccp( tabSize.width/2, tabSize.height/2 ));
	addChild(tabItems);


	tabCharacter->triggerGroup();

	TextField* tf = TextField::create(entity->getName());
	//CCTextFieldTTF* tf = CCTextFieldTTF::textFieldWithPlaceHolder(defaultTxt.c_str(), "Arial", 28);
	tf->setAnchorPoint(ccp(0.5,1));
	tf->setPosition(ccp(size.width/2, size.height - 50));
	tf->setEventBus("pme");
	addChild(tf);

	/*
	CCLabelTTF* title = CCLabelTTF::create("more people will come if we tell them we have punch and pie", "Arial", 24, CCSizeMake(400,100), kCCTextAlignmentCenter);
	title->setAnchorPoint(ccp(0.5,1));
	title->setPosition(ccp(size.width/2, size.height - 10));
	addChild(title);
	//*/


	return true;
}

void PartyMemberEditor::onTabSelect( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	int tabIdx = evt->json["index"].asInt();

	CCLog("todo: show tab select %d", tabIdx);

}

void PartyMemberEditor::onPMNameChange( CCObject* e )
{
	JsonEvent* evt = dynamic_cast<JsonEvent*>(e);
	if(!evt) return;

	TextField* tf = (TextField*) evt->json["pTextField"].asUInt();

	std::string name = tf->getString();

	if( name.size() > 2 )
		m_pEntity->setName( name );
}