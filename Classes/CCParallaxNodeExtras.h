#ifndef Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
#define Cocos2DxFirstIosSample_CCParallaxNodeExtras_h

#include "cocos2d.h"

using namespace cocos2d;

class CCParallaxNodeExtras:public CCParallaxNode{
public:

	CCParallaxNodeExtras();

	// later cast�� safety�� ���ϱ� ���� ����.
	static CCParallaxNodeExtras *node();

	//��� �Լ�
	//parallaxnode�� �ڽĵ��� ��ġ�� ������Ʈ �ϱ� ���� ���
	void incrementOffset(CCPoint offset, CCNode *node);
};

#endif
