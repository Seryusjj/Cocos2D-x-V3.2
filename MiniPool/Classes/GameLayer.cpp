
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

	//set up world and debug mode
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


	b2FixtureDef fixtureDef;
	b2BodyDef bodyDef;
	
	//Create table

	//add pockets
	b2Body * pocket;
	B2Sprite* pocketData;
	b2CircleShape circle;
	float startX = _screenSize.width * 0.07;
	float startY = _screenSize.height * 0.92f;
	for (int i = 0; i < 6; i++) {
		bodyDef.type = b2_staticBody;
		if (i < 3) {
			bodyDef.position.Set(
				startX / PTM_RATIO,
				(startY - i * (_screenSize.height * 0.84f * 0.5f)) / PTM_RATIO);

		}
		else {
			bodyDef.position.Set(
				(startX + _screenSize.width * 0.85f) / PTM_RATIO,
				(startY - (i - 3) * (_screenSize.height * 0.84f * 0.5f)) / PTM_RATIO);
		}
		pocket = _world->CreateBody(&bodyDef);
		fixtureDef.isSensor = true;
		circle.m_radius = (1.5 * BALL_RADIUS) / PTM_RATIO;
		fixtureDef.shape = &circle;

		pocket->CreateFixture(&fixtureDef);
		pocketData = new B2Sprite(this, kSpritePocket);
		pocket->SetUserData(pocketData);
		_pockets.pushBack(pocketData);
	}

	
	//create table sides
	b2BodyDef tableBodyDef;
	tableBodyDef.position.Set(0, 0);
	b2Body* tableBody = _world->CreateBody(&tableBodyDef);

	// Define the table edges
	b2EdgeShape tableBox;

	// bottom
	tableBox.Set(b2Vec2(_screenSize.width * 0.14f / PTM_RATIO, _screenSize.height * 0.09f / PTM_RATIO),
		b2Vec2(_screenSize.width * 0.86f / PTM_RATIO, _screenSize.height * 0.09f / PTM_RATIO));
	tableBody->CreateFixture(&tableBox, 0);

	// top
	tableBox.Set(b2Vec2(_screenSize.width * 0.14f / PTM_RATIO, _screenSize.height * 0.91f / PTM_RATIO),
		b2Vec2(_screenSize.width * 0.86f / PTM_RATIO, _screenSize.height * 0.91f / PTM_RATIO));
	tableBody->CreateFixture(&tableBox, 0);

	// left
	tableBox.Set(b2Vec2(_screenSize.width * 0.09f / PTM_RATIO, _screenSize.height * 0.12f / PTM_RATIO),
		b2Vec2(_screenSize.width * 0.09f / PTM_RATIO, _screenSize.height * 0.46f / PTM_RATIO));
	tableBody->CreateFixture(&tableBox, 0);

	tableBox.Set(b2Vec2(_screenSize.width * 0.91f / PTM_RATIO, _screenSize.height * 0.12f / PTM_RATIO),
		b2Vec2(_screenSize.width * 0.91f / PTM_RATIO, _screenSize.height * 0.46f / PTM_RATIO));
	tableBody->CreateFixture(&tableBox, 0);


	// right
	tableBox.Set(b2Vec2(_screenSize.width * 0.09f / PTM_RATIO, _screenSize.height * 0.54f / PTM_RATIO),
		b2Vec2(_screenSize.width * 0.09f / PTM_RATIO, _screenSize.height * 0.88f / PTM_RATIO));
	tableBody->CreateFixture(&tableBox, 0);


	tableBox.Set(b2Vec2(_screenSize.width * 0.91f / PTM_RATIO, _screenSize.height * 0.54f / PTM_RATIO),
		b2Vec2(_screenSize.width * 0.91f / PTM_RATIO, _screenSize.height * 0.88f / PTM_RATIO));
	tableBody->CreateFixture(&tableBox, 0);
	
}

void GameLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	Layer::draw(renderer, transform, flags);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	_customCommand.init(1000);
	_customCommand.func = CC_CALLBACK_0(GameLayer::onDraw, this,transform);
	renderer->addCommand(&_customCommand);
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}

void GameLayer::onDraw(const Mat4 &transform)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
	_world->DrawDebugData();
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}


void GameLayer::update(float dt) {

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	_world->Step(dt, 10, 10);
}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){ return true; }
void GameLayer::onTouchMoved(Touch *touch, Event *unused_event){}
void GameLayer::onTouchEnded(Touch *touch, Event *unused_event){}