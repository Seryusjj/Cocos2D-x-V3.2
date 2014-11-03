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

GameLayer::~GameLayer() {
	CC_SAFE_RELEASE(growBomb);
	CC_SAFE_RELEASE(rotateSprite);
	CC_SAFE_RELEASE(shockwaveSequence);
	CC_SAFE_RELEASE(swingHealth);
	CC_SAFE_RELEASE(groundHit);
	CC_SAFE_RELEASE(explosion);
	//release all retained arrays
	clouds.~Vector();
	meteorPool.~Vector();
	healthPool.~Vector();
	fallingObjects.~Vector();


}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//get screen size
	screenSize = Director::getInstance()->getVisibleSize();
	running = false;

	//create game screen elements
	createGameScreen();

	//create object pools
	createPools();
	//create Actions
	createActions();

	//create array to store all falling objects (will use it in collision check)
	fallingObjects = Vector<Node*>(40);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
	//lsiten for touches
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//start game loop
	this->schedule(schedule_selector(GameLayer::update));

	return true;
}

void GameLayer::update(float dt) {
	if (!running) return;
	//update timers

	meteorTimer += dt;
	if (meteorTimer > meteorInterval) {
		meteorTimer = 0;
		this->resetMeteor();
	}

	healthTimer += dt;
	if (healthTimer > healthInterval) {
		healthTimer = 0;
		this->resetHealth();
	}

	difficultyTimer += dt;
	if (difficultyTimer > difficultyInterval) {
		difficultyTimer = 0;
		this->increaseDifficulty();
	}
	int count;
	Node * sprite;
	//check collision with shockwave
	if (shockWave->isVisible()) {
		count = fallingObjects.size();
		for (int i = count - 1; i >= 0; i--) {
			sprite = fallingObjects.at(i);
			float diffx = shockWave->getPositionX() - sprite->getPositionX();
			float diffy = shockWave->getPositionY() - sprite->getPositionY();
			if (pow(diffx, 2) + pow(diffy, 2) <= pow(shockWave->getBoundingBox().size.width * 0.5f, 2)) {
				sprite->stopAllActions();
				sprite->runAction(explosion->clone());
				SimpleAudioEngine::getInstance()->playEffect("boom.wav");
				if (sprite->getTag() == SpriteMeteor) {
					shockwaveHits++;
					score += shockwaveHits * 13 + shockwaveHits * 2;
				}
				//play sound
				fallingObjects.erase(i);
			}
		}
		scoreDisplay->setString(std::to_string(score));
	}
	//move clouds
	count = clouds.size();
	for (int i = 0; i < count; i++) {
		sprite = clouds.at(i);
		sprite->setPositionX(sprite->getPositionX() + dt * 20);
		if (sprite->getPositionX() > screenSize.width + sprite->getBoundingBox().size.width * 0.5f) {
			sprite->setPositionX(sprite->getBoundingBox().size.width * 0.5f);
		}
	}

	if (bomb->isVisible()) {
		if (bomb->getScale() > 0.3f) {
			if (bomb->getOpacity() != 255)
				bomb->setOpacity(255);
		}
	}
}

//use a new health drop from the pool
void GameLayer::resetHealth() {
	if (fallingObjects.size() > 30) return;

	Node * health = healthPool.at(meteorPoolIndex);
	healthPoolIndex++;
	if (healthPoolIndex == healthPool.size()) healthPoolIndex = 0;

	int health_x = rand() % (int)(screenSize.width * 0.8f) + screenSize.width * 0.1f;
	int health_target_x = rand() % (int)(screenSize.width * 0.8f) + screenSize.width * 0.1f;

	health->stopAllActions();
	health->setPosition(Point(health_x, screenSize.height + health->getBoundingBox().size.height * 0.5));

	//create action (swing, move to target, and call function when done)
	FiniteTimeAction*  sequence = Sequence::create(
		MoveTo::create(healthSpeed, Point(health_target_x, screenSize.height * 0.15f)),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::fallingObjectDone, this)), NULL);

	health->setVisible(true);
	health->runAction((Action *)swingHealth->clone());
	health->runAction(sequence);
	fallingObjects.insert(fallingObjects.size(), health);
}


bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if (!running) {
		//if intro, hide intro message
		if (introMessage->isVisible()) {
			introMessage->setVisible(false);

		}//if game over, hide game over message
		else if (gameOverMessage->isVisible()) {
			SimpleAudioEngine::getInstance()->stopAllEffects();
			gameOverMessage->setVisible(false);
		}
		this->resetGame();
		return true;
	}

	if (touch) {
		//if bomb already growing...
		if (bomb->isVisible()) {
			//stop all actions on bomb, halo and sparkle
			bomb->stopAllActions();
			Node * child;
			child = bomb->getChildByTag(SpriteHalo);
			child->stopAllActions();
			child = bomb->getChildByTag(SpriteSparkle);
			child->stopAllActions();
			//if bomb is the right size, then create shockwave
			if (bomb->getScale() > 0.3f) {
				shockWave->setScale(0.1f);
				shockWave->setOpacity(255);
				shockWave->setPosition(bomb->getPosition());
				shockWave->setVisible(true);
				shockWave->runAction(ScaleTo::create(0.5f, bomb->getScale() * 2.0f));
				shockWave->runAction(shockwaveSequence->clone());
				SimpleAudioEngine::getInstance()->playEffect("bombRelease.wav");
			}
			else {
				SimpleAudioEngine::getInstance()->playEffect("bombFail.wav");
			}
			bomb->setVisible(false);
			//reset hits with shockwave, so we can count combo hits
			shockwaveHits = 0;
			//if no bomb currently on screen, create one
		}
		else {
			Point tap = touch->getLocation();
			bomb->stopAllActions();
			bomb->setScale(0.1f);
			bomb->setPosition(tap);
			bomb->setVisible(true);
			bomb->setOpacity(50);
			//clone hace autorelease automatico
			bomb->runAction(growBomb->clone());
			Node * child;
			child = bomb->getChildByTag(SpriteHalo);
			child->runAction(rotateSprite->clone());
			child = bomb->getChildByTag(SpriteSparkle);
			child->runAction(rotateSprite->clone());
		}
	}
	return true;
}

//call back for when falling object reaches its target
void GameLayer::fallingObjectDone(Node* pSender) {

	//remove it from array
	fallingObjects.eraseObject(pSender);
	pSender->stopAllActions();
	pSender->setRotation(0);

	//if object is a meteor...

	if (pSender->getTag() == SpriteMeteor) {
		energy -= 15;
		//show explosion animation
		pSender->runAction(groundHit->clone());
		//play explosion sound
		SimpleAudioEngine::getInstance()->playEffect("boom.wav");

		//if object is a health drop...
	}
	else {
		pSender->setVisible(false);

		//if energy is full, score points from falling drop
		if (energy == 100) {

			score += 25;
			char score[100] = { 0 };
			sprintf(score, "%i", score);
			scoreDisplay->setString(score);

		}
		else {
			energy += 10;
			if (energy > 100) energy = 100;
		}

		//play health bonus sound
		SimpleAudioEngine::getInstance()->playEffect("health.wav");
	}

	//if energy is less or equal 0, game over
	if (energy <= 0) {
		energy = 0;
		this->stopGame();
		SimpleAudioEngine::getInstance()->playEffect("fire_truck.wav");
		//show GameOver
		gameOverMessage->setVisible(true);
	}

	
	energyDisplay->setString(std::to_string(energy));

}


//call back for animation done (hide animated object)
void GameLayer::animationDone(Node* pSender) {
	pSender->setVisible(false);
}

//call back for shockwave done, hide shockwave
void GameLayer::shockwaveDone() {
	shockWave->setVisible(false);
}

//use a new meteor from the pool
void GameLayer::resetMeteor() {
	if (fallingObjects.size() > 30) return;
	Node * meteor = meteorPool.at(meteorPoolIndex);
	meteorPoolIndex++;
	if (meteorPoolIndex == meteorPool.size())
		meteorPoolIndex = 0;
	//pick start and target positions for new meteor
	int meteor_x = rand() % (int)(screenSize.width * 0.8f) +
		screenSize.width * 0.1f;
	int meteor_target_x = rand() % (int)
		(screenSize.width * 0.8f) + screenSize.width * 0.1f;
	meteor->stopAllActions();
	meteor->setPosition(Point(meteor_x, screenSize.height + meteor->getBoundingBox().size.height * 0.5));
	//create action for meteor
	ActionInterval* rotate = RotateBy::create(0.5f, -90);
	Action* repeatRotate = RepeatForever::create(rotate);
	FiniteTimeAction* sequence = Sequence::create(MoveTo::create(meteorSpeed, Point(meteor_target_x, screenSize.height * 0.15f)),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::fallingObjectDone, this)), NULL);
	meteor->setVisible(true);
	meteor->runAction(repeatRotate);
	meteor->runAction(sequence);
	fallingObjects.pushBack(meteor);
}
//start
void GameLayer::resetGame() {
	score = 0;
	energy = 100;
	//reset timers and "speeds"
	meteorInterval = 2.5;
	meteorTimer = meteorInterval * 0.99f;
	meteorSpeed = 10;//in seconds to reach ground
	healthInterval = 20;
	healthTimer = 0;
	healthSpeed = 15;//in seconds to reach ground
	difficultyInterval = 60;
	difficultyTimer = 0;
	running = true;
	//reset labels
	std::string value = (std::to_string(energy) + "%");
	energyDisplay->setString(value);
	value = std::to_string(score);
	scoreDisplay->setString(value);
}

void GameLayer::increaseDifficulty() {

	meteorInterval -= 0.2f;
	if (meteorInterval < 0.25f) meteorInterval = 0.25f;
	meteorSpeed -= 1;
	if (meteorSpeed < 4) meteorSpeed = 4;
	healthSpeed -= 1;
	if (healthSpeed < 8) healthSpeed = 8;

}

//if player is killed, stop all actions, clear screen
void GameLayer::stopGame() {
	running = false;
	//stop all actions currently running
	Node * sprite;
	for (int i = fallingObjects.size() - 1; i >= 0; i--) {
		sprite = fallingObjects.at(i);
		sprite->stopAllActions();
		sprite->setVisible(false);
		fallingObjects.erase(i);
	}
	if (bomb->isVisible()) {
		bomb->stopAllActions();
		bomb->setVisible(false);
		Node * child;
		child = bomb->getChildByTag(SpriteHalo);
		child->stopAllActions();
		child =bomb->getChildByTag(SpriteSparkle);
		child->stopAllActions();
	}
	if (shockWave->isVisible()) {
		shockWave->stopAllActions();
		shockWave->setVisible(false);
	}
}



void GameLayer::createGameScreen() {
	//SCENE
	//add bg
	Sprite * bg = AssetsManager::getInstance()->getBackground();
	bg->setPosition(Point(screenSize.width * 0.5f, screenSize.height * 0.5f));
	this->addChild(bg);

	SpriteFrameCache::getInstance()->
		addSpriteFramesWithFile("sprite_sheet.plist");
	gameBatchNode = SpriteBatchNode::create("sprite_sheet.png");
	this->addChild(gameBatchNode);

	Sprite* sprite;
	for (int i = 0; i < 2; i++){
		sprite = Sprite::createWithSpriteFrameName("city_dark.png");
		sprite->setPosition(Point(screenSize.width*(0.25f + i*0.5f),
			sprite->getBoundingBox().size.height*0.5f));
		gameBatchNode->addChild(sprite, Foreground);

		sprite = Sprite::createWithSpriteFrameName("city_light.png");
		sprite->setPosition(Point(screenSize.width*(0.25f + i*0.5f), sprite->getBoundingBox().size.height*0.9f));
		gameBatchNode->addChild(sprite, Background);
	}

	//add trees
	for (int i = 0; i < 3; i++){
		sprite = Sprite::createWithSpriteFrameName("trees.png");
		sprite->setPosition(Point(screenSize.width*(0.2f + i*0.3f),
			sprite->getBoundingBox().size.height*0.5f));
		gameBatchNode->addChild(sprite, Foreground);
	}


	//add clouds
	clouds = Vector<Node*>(4);
	float cloud_y;
	for (int i = 0; i < 4; i++) {
		cloud_y = i % 2 == 0 ? screenSize.height * 0.4f : screenSize.height * 0.5f;
		sprite = Sprite::createWithSpriteFrameName("cloud.png");
		sprite->setPosition(Point(screenSize.width * 0.1f + i *
			screenSize.width * 0.3f, cloud_y));
		gameBatchNode->addChild(sprite, Background);
		clouds.insert(i, sprite);
	}
	//add bomb
	bomb = Sprite::createWithSpriteFrameName("bomb.png");
	//generateMipmap is antialiasing (allow resize without lose quality)
	bomb->getTexture()->generateMipmap();
	bomb->setVisible(false);
	Size size = bomb->getBoundingBox().size;
	//add sparkle inside bomb sprite
	Sprite * sparkle = Sprite::createWithSpriteFrameName("sparkle.png");
	sparkle->setPosition(Point(size.width * 0.72f, size.height *0.72f));
	bomb->addChild(sparkle, Middleground, SpriteSparkle);

	//add halo inside bomb sprite
	Sprite * halo = Sprite::createWithSpriteFrameName("halo.png");
	halo->setPosition(Point(size.width * 0.4f, size.height *	0.4f));
	bomb->addChild(halo, Middleground, SpriteHalo);
	gameBatchNode->addChild(bomb, Foreground);

	shockWave = CCSprite::createWithSpriteFrameName("shockwave.png");
	shockWave->getTexture()->generateMipmap();
	shockWave->setVisible(false);
	gameBatchNode->addChild(shockWave);


	//UI

	//create bitmap font and set a text
	//loand the file descriptor that tels where the .png is and how to use it

	scoreDisplay = Label::createWithBMFont("font.fnt", "0");
	scoreDisplay->setAnchorPoint(Point(1, 0.5));
	//anchor point pone el punto desde el que se mueve la imagen en 
	//  000
	//  001
	//  000
	scoreDisplay->setPosition(Point(screenSize.width * 0.8f, screenSize.height * 0.94f));
	this->addChild(scoreDisplay);

	energyDisplay = Label::createWithBMFont("font.fnt", "100%", TextHAlignment::RIGHT);//, screenSize.width * 0.1f, TextHAlignment::RIGHT);
	energyDisplay->setPosition(Point(screenSize.width * 0.3f, screenSize.height * 0.94f));
	this->addChild(energyDisplay);

	//icon 
	Sprite * icon = Sprite::createWithSpriteFrameName("health_icon.png");
	icon->setPosition(Point(screenSize.width * 0.15f, screenSize.height * 0.94f));
	gameBatchNode->addChild(icon, Background);


	introMessage = Sprite::createWithSpriteFrameName("logo.png");
	introMessage->setPosition(Point(screenSize.width * 0.5f, screenSize.height * 0.6f));
	introMessage->setVisible(true);
	this->addChild(introMessage, Foreground);
	gameOverMessage = Sprite::createWithSpriteFrameName("gameover.png");
	gameOverMessage->setPosition(Point(screenSize.width * 0.5f, screenSize.height * 0.65f));
	gameOverMessage->setVisible(false);
	this->addChild(gameOverMessage, Foreground);
}

void GameLayer::createPools() {
	meteorPool = Vector<Node*>(50);
	Sprite* sprite;
	meteorPoolIndex = 0;
	while (meteorPoolIndex < 50) {
		sprite = Sprite::createWithSpriteFrameName("meteor.png");
		sprite->setVisible(false);
		gameBatchNode->addChild(sprite, Middleground, SpriteMeteor);
		meteorPool.insert(meteorPoolIndex, sprite);
		meteorPoolIndex++;
	}
	meteorPoolIndex = 0;
	healthPool = Vector<Node*>(20);
	healthPoolIndex = 0;
	while (healthPoolIndex < 20) {
		sprite = Sprite::createWithSpriteFrameName("meteor.png");
		sprite->setVisible(false);
		sprite->setAnchorPoint(Point(0.5f, 0.8f));
		gameBatchNode->addChild(sprite, Middleground, SpriteHealth);
		healthPool.insert(healthPoolIndex, sprite);
		healthPoolIndex++;
	}
	healthPoolIndex = 0;

}

void GameLayer::createActions() {
	//swing action for health drops
	ActionInterval* easeSwing = Sequence::create(EaseInOut::create(RotateTo::create(1.2f, -10), 2),
		EaseInOut::create(RotateTo::create(1.2f, 10), 2), NULL);

	swingHealth = RepeatForever::create(easeSwing);
	swingHealth->retain();

	//action sequence for shockwave: fade out, callback when
	//done

	shockwaveSequence = Sequence::create(FadeOut::create(1.0f),
		// CC_CALLBACK0 is substituting callfunc_selector
		CallFunc::create(CC_CALLBACK_0(GameLayer::shockwaveDone, this)), NULL);
	
	shockwaveSequence->retain();
	//action to grow bomb
	growBomb = ScaleTo::create(6.0f, 1.0);
	growBomb->retain();
	//action to rotate sprites
	ActionInterval* rotate = RotateBy::create(0.5f, -90);
	rotateSprite = RepeatForever::create(rotate);
	rotateSprite->retain();

	//create animation
	Animation* animation;
	SpriteFrame * frame;
	//create Animation object
	animation = Animation::create();
	std::string name = "";
	for (int i = 1; i <= 10; i++) {
		name.append("boom");
		name.append(std::to_string(i));
		name.append(".png");
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		animation->addSpriteFrame(frame);
		name = "";
	}
	animation->setDelayPerUnit(1 / 10.0f);
	animation->setRestoreOriginalFrame(true);

	groundHit = Sequence::create(MoveBy::create(0, Point(0, screenSize.height * 0.12f)),
		Animate::create(animation), CallFuncN::create(CC_CALLBACK_1(GameLayer::animationDone, this)), NULL);

	groundHit->retain();

	animation = Animation::create();
	for (int i = 1; i <= 7; i++) {
		name.append("explosion_small");
		name.append(std::to_string(i));
		name.append(".png");
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		animation->addSpriteFrame(frame);
		name = "";
	}
	animation->setDelayPerUnit(0.5f / 7.0f);
	animation->setRestoreOriginalFrame(true);
	explosion = Sequence::create(Animate::create(animation),
		//CC_CALLBACK1 substite callfuncN_selector
		CallFuncN::create(CC_CALLBACK_1(GameLayer::animationDone, this)), NULL);

	explosion->retain();
}


