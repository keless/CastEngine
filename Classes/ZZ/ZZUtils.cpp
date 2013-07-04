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

}