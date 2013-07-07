#include "ZZTextField.h"


namespace ZZ {

TextField::TextField(void)
{
	m_tf = NULL;
}


TextField::~TextField(void)
{
}


//static 
TextField* TextField::create( std::string defaultText, std::string fontName, int size, const CCSize& dimensions, CCTextAlignment alignment )
{
	
	TextField* tf = new TextField();
	if(!tf->init( defaultText, fontName, size, dimensions, alignment ) )
	{
		delete tf;
		return NULL;
	}
	return tf;
}

//virtual 
bool TextField::init( std::string defaultText, std::string fontName, int size, const CCSize& dimensions, CCTextAlignment alignment )
{
	m_defaultText = defaultText; 
	m_tf = CCTextFieldTTF::textFieldWithPlaceHolder(defaultText.c_str(), dimensions, alignment, fontName.c_str(), size);
	addChild(m_tf);

	ignoreAnchorPointForPosition(false);
	setTouchEnabled(true);
	setTouchMode( kCCTouchesOneByOne );

	return true;
}

//virtual
void TextField::setString(const char *label)
{
	m_tf->setString(label);
}
//virtual 
const char* TextField::getString(void)
{
	return m_tf->getString();
}

//virtual 
bool TextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint p = pTouch->getLocation();
	//todo: convert to local coords

	CCPoint local = convertToNodeSpace(p);

	for( int i=0; i< m_pChildren->count(); i++)
	{
		CCNode* child = (CCNode*) m_pChildren->objectAtIndex(i);
		if( child->boundingBox().containsPoint( local ) ) {
			m_touchStarted = true;
			return true;
		}
	}

	return false;
}
//virtual 
void TextField::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint p = pTouch->getLocation();
	//todo: convert to local coords

	CCPoint local = convertToNodeSpace(p);

	bool movedInside = false;

	for( int i=0; i< m_pChildren->count(); i++)
	{
		CCNode* child = (CCNode*) m_pChildren->objectAtIndex(i);
		if( child->boundingBox().containsPoint( local ) ) {
			movedInside = true;
			break;
		}
	}
}
//virtual 
void TextField::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint p = pTouch->getLocation();
	//todo: convert to local coords

	CCPoint local = convertToNodeSpace(p);

	bool clickedInside = false;

	for( int i=0; i< m_pChildren->count(); i++)
	{
		CCNode* child = (CCNode*) m_pChildren->objectAtIndex(i);
		if( child->boundingBox().containsPoint( local ) ) {
			//touched inside

			//handle touch event
			clickedInside = true;
			break;
		}
	}

	if( clickedInside ) {
		CCLog("start text entry");

		if( m_defaultText.compare( m_tf->getString() ) == 0 )
		{
			m_tf->setString(""); //clear default string on entry
		}
		
		m_tf->attachWithIME();
	}else {
		CCLog("quit text entry");
		m_tf->detachWithIME();
	}
	m_touchStarted = false;
}

//virtual 
void TextField::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	m_touchStarted = false;
}

}