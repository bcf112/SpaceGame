#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "CCParallaxNodeExtras.h"

using namespace cocos2d;

//���� ����
typedef enum{
	KENDREASONWIN,
	KENDREASONLOSE
}EndReason;

typedef enum{
	KSHIPSTATEGRAPPED,
	KSHIPSTAGEUNGRAPPED
} ShipState;

class HelloWorld : public cocos2d::CCLayer
{
private:
	CCSpriteBatchNode *_batchNode;
	CCSprite *_ship;

	CCParallaxNodeExtras *_backgroundNode;
	CCSprite *_spacedust1;
	CCSprite *_spacedust2;
	CCSprite *_planetsunrise;
	CCSprite *_galaxy;
	CCSprite *_spacialanomaly;
	CCSprite *_spacialanomaly2;

	void update(float dt);

	//���Ӱ�� ���ּ� �����̱�
	float _shipPointsPerSecY;
	virtual void didAccelerate(CCAcceleration *pAccelerationValue);

	//� �߰�
	CCArray *_asteroids;
	unsigned int _nextAsteroid;
	float _nextAsteroidSpawn;

	//������ ���
	CCArray *_shipLasers;
	int _nextShipLaser;

	//�浹����
	int _lives;

	//��������
	double _gameOverTime;
	bool _gameOver;
	void endScene(EndReason endReason);
	void restartTapped(CCObject *object);

	//���ּ� ����
	ShipState _shipState;
	
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	//� �߰�
	float randomValueBetween(float low, float high);
	void setInvisible(CCNode *node);
	float getTimeTick();

	//������ ���
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
};

#endif  // __HELLOWORLD_SCENE_H__