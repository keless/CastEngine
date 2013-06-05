#ifndef _ZZPROGRESSBAR_H_
#define _ZZPROGRESSBAR_H_

#include <cocos2d.h>

using namespace cocos2d;

class ZZProgressBar : public CCNode
{
	CCLayerColor* m_bg;
	CCLayerColor* m_bar;
	float m_margin;

public:
	ZZProgressBar(void);
	~ZZProgressBar(void);

	virtual bool init(  const CCRect &area );

	static ZZProgressBar * create( const CCRect &area );

	void setProgress( float pct );
	void setMargin( float margin );
};

#endif
