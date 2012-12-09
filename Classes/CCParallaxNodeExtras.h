#ifndef Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
#define Cocos2DxFirstIosSample_CCParallaxNodeExtras_h

#include "cocos2d.h"

using namespace cocos2d;

class CCParallaxNodeExtras:public CCParallaxNode{
public:

	CCParallaxNodeExtras();

	// later cast와 safety를 피하기 위한 것임.
	static CCParallaxNodeExtras *node();

	//기능 함수
	//parallaxnode의 자식들의 위치를 업데이트 하기 위해 사용
	void incrementOffset(CCPoint offset, CCNode *node);
};

#endif
