#include "GameLayer.h"



Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameLayer::~GameLayer(){}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	//init game values
	_screenSize = Director::sharedDirector()->getWinSize();
	_drawing = false;
	_minLineLength = _screenSize.width * 0.07f;
	_state = gameIntro;

	createGameScreen();

	createParticles();

	createStarGrid();

	//listen for touches
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//create main loop
	this->schedule(schedule_selector(GameLayer::update));
    
    return true;
}
void GameLayer::update(float dt){}
bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){ return true; }
void GameLayer::onTouchMoved(Touch *touch, Event *unused_event){ return; }
void GameLayer::onTouchEnded(Touch *touch, Event *unused_event){ return; }

void GameLayer::resetGame(){}
void GameLayer::resetStar(){}
void GameLayer::killPlayer(){}

void GameLayer::createGameScreen(){}
void GameLayer::createParticles(){}
void GameLayer::createStarGrid(){}

