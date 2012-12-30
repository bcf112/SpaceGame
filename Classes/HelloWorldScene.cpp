#define KNUMASTEROIDS 15
#define KNUMLASERS 5

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

		//운석 추가
		_asteroids=new CCArray();
		for(int i=0; i<KNUMASTEROIDS; i++){
			CCSprite *asteroid=CCSprite::createWithSpriteFrameName("asteroid.png");
			asteroid->setVisible(false);
			_batchNode->addChild(asteroid);
			_asteroids->addObject(asteroid);
		}
		_nextAsteroid=0;

		//레이저 쏘기
		_shipLasers=new CCArray();
		for(int i=0; i<KNUMLASERS; i++){
			CCSprite *shipLaser=CCSprite::createWithSpriteFrameName("laserbeam_blue.png");
			shipLaser->setVisible(false);
			_batchNode->addChild(shipLaser);
			_shipLasers->addObject(shipLaser);
		}
		this->setTouchEnabled(true);
		_nextShipLaser=0;

		//승패 판정
		_lives=3;
		double curTime=getTimeTick();
		_gameOverTime=curTime+30000;
		_gameOver=false;


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


	//Accelometer로 비행선 움직이기
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	//float maxY=winSize.height - _ship->getContentSize().height/2;
	//float minY=_ship->getContentSize().height/2;

	//float diff=(_shipPointsPerSecY * dt);
	//float newY=_ship->getPosition().y + diff;
	//newY=MIN(MAX(newY, minY), maxY);
	//_ship->setPosition(ccp(_ship->getPosition().x, newY));


	//운석 추가
	float curTimeMillis = getTimeTick();
	if(curTimeMillis > _nextAsteroidSpawn) {
		float randMillisecs = randomValueBetween(0.20, 1.0) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;

		float randY = randomValueBetween(30.0, winSize.height);
		float randDuration = randomValueBetween(2.0, 10.0);

		CCSprite *asteroid = (CCSprite *)_asteroids->objectAtIndex(_nextAsteroid);
		_nextAsteroid++;

		if(_nextAsteroid >= _asteroids->count()) {
			_nextAsteroid = 0;
		}

		asteroid->stopAllActions();
		asteroid->setPosition(ccp(winSize.width + asteroid->getContentSize().width/2, randY));
		asteroid->setVisible(true);
		asteroid->runAction(CCSequence::create(CCMoveBy::create(randDuration, ccp(-winSize.width - asteroid->getContentSize().width, 0)),
			CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)), NULL));
	}

	CCObject *asteroid, *shipLaser;
	//충돌판정
	CCARRAY_FOREACH(_asteroids, asteroid)
	{
		CCSprite *s_asteroid=(CCSprite *)asteroid;
		if(!s_asteroid->isVisible()){
			continue;
		} 
		CCARRAY_FOREACH(_shipLasers, shipLaser)
		{
			CCSprite *s_shipLaser=(CCSprite *)shipLaser;
			if(!s_shipLaser->isVisible()){
				continue;
			}
			if(s_shipLaser->boundingBox().intersectsRect(s_asteroid->boundingBox())){
				s_shipLaser->setVisible(false);
				s_asteroid->setVisible(false);
				continue;
			}
		}

		if(_ship->boundingBox().intersectsRect(s_asteroid->boundingBox())){
			s_asteroid->setVisible(false);
			_ship->runAction(CCBlink::create(1.0, 9));
			_lives--;
		}
	}


	//승패판정
	if(_lives <= 0){
		_ship->stopAllActions();
		_ship->setVisible(false);
		this->endScene(KENDREASONLOSE);
	}else if(curTimeMillis >= _gameOverTime){
		this->endScene(KENDREASONWIN);
	}
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

float HelloWorld::randomValueBetween(float low, float high){
	//mac이 아니면 arc4random()함수가 없어서 이렇게 해야된다.
	return ((rand() % (int)(high - low+1))+1) + low;
}

float HelloWorld::getTimeTick(){
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
}

void HelloWorld::setInvisible(CCNode *node){
	node->setVisible(false);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCRect shipBoundingBox = _ship->boundingBox();

	if(shipBoundingBox.containsPoint(convertTouchToNodeSpace(touch))) {
		_shipState = KSHIPSTATEGRAPPED;
	}else {
		_shipState = KSHIPSTAGEUNGRAPPED;
		CCSprite *shipLaser=(CCSprite *)_shipLasers->objectAtIndex(_nextShipLaser++);
		if(_nextShipLaser >= _shipLasers->count()){
			_nextShipLaser = 0;
		}
		shipLaser->setPosition(ccpAdd(_ship->getPosition(), ccp(shipLaser->getContentSize().width/2, 0)));
		shipLaser->setVisible(true);
		shipLaser->stopAllActions();
		shipLaser->runAction(CCSequence::create(CCMoveBy::create(0.5, ccp(winSize.width, 0)),
			CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)),
			NULL));
	}

	CCLOG("shipState : %d", _shipState); 
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {
	if(_shipState == KSHIPSTATEGRAPPED) {
		CCTouch *touch = (CCTouch *)pTouches->anyObject();
		CCPoint touchPoint = touch->getLocation();
		_ship->setPosition(ccp(_ship->getPosition().x, touchPoint.y));
	}
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {

}

void HelloWorld::restartTapped(CCObject *object){
	CCDirector::sharedDirector()->replaceScene(CCTransitionZoomFlipX::create(0.5, this->scene()));

	//리스케줄
	this->scheduleUpdate();
}

void HelloWorld::endScene(EndReason endReason){
	if(_gameOver){
		return;
	}
	_gameOver=true;

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	char message[10]="You Win";
	if(endReason == KENDREASONLOSE){
		strcpy(message, "You Lose");
	}

	CCLabelBMFont *label=CCLabelBMFont::create(message, "Arial.fnt");
	label->setScale(0.1);
	label->setPosition(ccp(winSize.width/2, winSize.height*0.6));
	this->addChild(label);

	CCLabelBMFont *restartLabel = CCLabelBMFont::create("Restart", "Arial.fnt");
	CCMenuItemLabel *restartItem = CCMenuItemLabel::create(restartLabel, this, menu_selector(HelloWorld::restartTapped));
	restartItem->setScale(0.1);
	restartItem->setPosition(ccp(winSize.width/2, winSize.height*0.4));

	CCMenu *menu=CCMenu::create(restartItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	//clear label and menu
	restartItem->runAction(CCScaleTo::create(0.5, 1.0));
	label->runAction(CCScaleTo::create(0.5, 1.0));
	this->unscheduleUpdate();
}

