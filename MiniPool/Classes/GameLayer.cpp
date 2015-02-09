
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

// on "init" you need to initialize your instance
bool GameLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	initPhysics();

	//listen for touch, not multi touch app in this case.
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//create main loop
	this->schedule(schedule_selector(GameLayer::update));

	return true;
}
GameLayer::~GameLayer(){
	_balls.~Vector();
	_pockets.~Vector();
	delete _debugDraw;
	_debugDraw = nullptr;
	delete _world;
	_world = nullptr;


}
GameLayer::GameLayer(){}

void GameLayer::placeCue(Point position){}
void GameLayer::resetGame(){}

void GameLayer::ticktock(){}

void GameLayer::initPhysics(){

	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	_world = new b2World(gravity);

	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);
	_collisionListener = new CollisionListener();
	_world->SetContactListener(_collisionListener);

	_debugDraw = new GLESDebugDraw(PTM_RATIO);
	_world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	_debugDraw->SetFlags(flags);
}

void GameLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
	//super.draw
	Layer::draw(renderer, transform, flags);
	//pre-render
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
	//render now
	//renderer->add
}

void GameLayer::update(float dt) {}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){ return true; }
void GameLayer::onTouchMoved(Touch *touch, Event *unused_event){}
void GameLayer::onTouchEnded(Touch *touch, Event *unused_event){}