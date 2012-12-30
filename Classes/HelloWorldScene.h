#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "CCParallaxNodeExtras.h"

using namespace cocos2d;

//승패 판정
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

	//가속계로 우주선 움직이기
	float _shipPointsPerSecY;
	virtual void didAccelerate(CCAcceleration *pAccelerationValue);

	//운석 추가
	CCArray *_asteroids;
	unsigned int _nextAsteroid;
	float _nextAsteroidSpawn;

	//레이저 쏘기
	CCArray *_shipLasers;
	int _nextShipLaser;

	//충돌판정
	int _lives;

	//승패판정
	double _gameOverTime;
	bool _gameOver;
	void endScene(EndReason endReason);
	void restartTapped(CCObject *object);

	//우주선 상태
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

	//운석 추가
	float randomValueBetween(float low, float high);
	void setInvisible(CCNode *node);
	float getTimeTick();

	//레이저 쏘기
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
};

#endif  // __HELLOWORLD_SCENE_H__