#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);
		


		//SpriteBatchNode에 우주선을 추가하는 과정
		_batchNode=CCSpriteBatchNode::create("Sprites.pvr.ccz");
		this->addChild(_batchNode);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");

		_ship=CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		_ship->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
		_batchNode->addChild(_ship, 1);

		// 1)CCParallaxNode 만들기
		_backgroundNode=CCParallaxNodeExtras::node();
		this->addChild(_backgroundNode, -1);

		// 2)Sprites를 생성하고, CCParallaxNode에 추가하기
		_spacedust1=CCSprite::create("bg_front_spacedust.png");
		_spacedust2=CCSprite::create("bg_front_spacedust.png");
		_planetsunrise=CCSprite::create("bg_planetsunrise.png");
		_galaxy=CCSprite::create("bg_galaxy.png");
		_spacialanomaly=CCSprite::create("bg_spacialanomaly.png");
		_spacialanomaly2=CCSprite::create("bg_spacialanomaly2.png");

		// 3)spacedust와 background 사이의 상대적인 움직임 속도 정하기
		CCPoint dustSpeed=ccp(0.1, 0.1);
		CCPoint bgSpeed=ccp(0.05, 0.05);

		// 4)CCParallaxNode에 자식 추가하기
		_backgroundNode->addChild(_spacedust1, 0, dustSpeed, ccp(0, winSize.height/2));
		_backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp(_spacedust1->getContentSize().width, winSize.height/2));
		_backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(0, winSize.height * 0.7));
		_backgroundNode->addChild(_planetsunrise, -1, bgSpeed, ccp(600, winSize.height * 0));
		_backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900, winSize.height * 0.3));
		_backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500, winSize.height * 0.9));

		//별 추가하기
		this->addChild(CCParticleSystemQuad::create("Stars1.plist"));
		this->addChild(CCParticleSystemQuad::create("Stars2.plist"));
		this->addChild(CCParticleSystemQuad::create("Stars3.plist"));

		//Accelerometer로 우주선 움직이기
		this->setAccelerometerEnabled(true);

		bRet=true;
    } while (0);

	this->scheduleUpdate();
	return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::update(float dt){
	CCPoint backgroundScrollVert=ccp(-1000, 0);
	_backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));

	CCArray *spaceDusts=CCArray::createWithCapacity(2);
	spaceDusts->addObject(_spacedust1);
	spaceDusts->addObject(_spacedust2);

	//backgroundNode의 자식 객체들은 좌측으로 이동하면서 위치좌표가 -가 된다.
	//이 때 spaceDust의 크기를 - 한 값보다 전체 화면에서의 xPosition이 작게 되면
	//spaceDust의 위치를 spaceDust의 크기를 곱하기 2해서 원래 위치로 돌려놓는다.
	for(unsigned int i=0; i < spaceDusts->count(); i++){
		CCSprite *spaceDust=(CCSprite *)(spaceDusts->objectAtIndex(i));
		float xPosition=_backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
		float size=spaceDust->getContentSize().width;
		if(xPosition < -size){
			_backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0), spaceDust);
		}
	}

	CCArray *backGrounds=CCArray::createWithCapacity(4);
	backGrounds->addObject(_galaxy);
	backGrounds->addObject(_planetsunrise);
	backGrounds->addObject(_spacialanomaly);
	backGrounds->addObject(_spacialanomaly2);

	for(unsigned int ii=0; ii<backGrounds->count(); ii++){
		CCSprite *background=(CCSprite *)(backGrounds->objectAtIndex(ii));
		float xPosition=_backgroundNode->convertToWorldSpace(background->getPosition()).x;
		float size=background->getContentSize().width;
		if(xPosition < -size){
			_backgroundNode->incrementOffset(ccp(2000, 0), background);
		}
	}



	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float maxY=winSize.height - _ship->getContentSize().height/2;
	float minY=_ship->getContentSize().height/2;

	float diff=(_shipPointsPerSecY * dt);
	float newY=_ship->getPosition().y + diff;
	newY=MIN(MAX(newY, minY), maxY);
	_ship->setPosition(ccp(_ship->getPosition().x, newY));
}

void HelloWorld::didAccelerate(cocos2d::CCAcceleration *pAccelerationValue){
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height * 0.5)
#define KMAXDIFFX 0.2

	double rollingX;

	pAccelerationValue->x=pAccelerationValue->y;
	rollingX=(pAccelerationValue->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
	float accelX=pAccelerationValue->x - rollingX;
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float accelDiff=accelX - KRESTACCELX;
	float accelFraction=accelDiff / KMAXDIFFX;
	_shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;
}

