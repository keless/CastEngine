#include "ZZUtils.h"


namespace ZZ {

Json::Value ReadFileToJson( std::string fileName )
{
	std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());

	bool bRet = false;
    unsigned long size = 0;
    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &size);
    if (pBuffer == NULL || size <= 0)
    {
		return Json::Value();
        //bRet = parse(pBuffer, size);
    }
	std::string strJson(pBuffer, size);

	CC_SAFE_DELETE_ARRAY(pBuffer);

	Json::Value root;
	Json::Reader reader;
	reader.parse(strJson, root);

    return root;

}

CCLayer* CreateSimplePopup( std::string msg, std::string btnLabel, std::string okEvt, std::string bus )
{
	CCSize area = CCSizeMake(300,250);

	CCLayerGradient* pop = CCLayerGradient::create(ccc4(200,200,200,255), ccc4(150,150,150,255), ccp(0.3,1));
	pop->setContentSize(area);
	pop->ignoreAnchorPointForPosition(false);
	pop->setAnchorPoint(ccp(0.5,0.5));

	area.height -= 50;
	CCLabelTTF* label = CCLabelTTF::create(msg.c_str(), "Arial", 24, area, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
	label->setAnchorPoint(ccp(0,0));
	label->setPositionY(50);
	pop->addChild(label);

	TouchableNode* btn = CreateSimpleButton( btnLabel, okEvt, bus );
	btn->setKill(pop);
	btn->setAnchorPoint(ccp(0.5,0));
	btn->setPositionX( area.width/2);

	pop->addChild(btn);

	return pop;
}

TouchableNode* CreateSimpleButton( std::string msg, std::string evt, std::string bus )
{
	CCSize area = CCSizeMake(100, 40);

	CCLayerColor* btn = CCLayerColor::create(ccc4(50,150,50,255));
	btn->ignoreAnchorPointForPosition(false);
	btn->setContentSize(area);
	btn->setAnchorPoint(ccp(0,0));
	CCLabelTTF* btnLabel = CCLabelTTF::create(msg.c_str(), "Arial", 24, CCSizeMake(100,40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	btnLabel->setAnchorPoint(ccp(0,0));
	btn->addChild(btnLabel);

	TouchableNode* sBtn = new TouchableNode(evt, bus);
	sBtn->addChild(btn);
	sBtn->setAnchorPoint(ccp(0,0));
	sBtn->setPositionY(5);
	sBtn->setContentSize(area);


	return sBtn;
}


}