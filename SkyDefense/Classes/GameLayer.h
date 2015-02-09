#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include <iostream>
#include "cocos2d.h"
#include "FileNameOrganicer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

enum {
	SpriteBomb,
	SpriteShockwave,
	SpriteMeteor,
	SpriteHealth,
	SpriteHalo,
	SpriteSparkle
};

enum {
	Background,
	Middleground,
	Foreground
};

class GameLayer : public cocos2d::Layer
{
private://variables
	Size screenSize;

	SpriteBatchNode * gameBatchNode;

	Sprite* shockWave;
	Sprite* bomb;
	Sprite* introMessage;
	Sprite* gameOverMessage;

	Action* groundHit;
	Action* explosion;


	Action* swingHealth;
	Action * growBomb;
	Action * rotateSprite;
	Action * shockwaveSequence;



	Label* energyDisplay;
	Label* scoreDisplay;

	float meteorInterval;
	float meteorTimer;
	float meteorSpeed;
	float healthInterval;
	float healthTimer;
	float healthSpeed;
	float difficultyInterval;
	float difficultyTimer;

	int energy;
	int score;
	int shockwaveHits;

	

	bool running;

	Vector<Node*> clouds;
	Vector<Node*> fallingObjects;
	Vector<Node*> healthPool;
	int healthPoolIndex;
	Vector<Node*> meteorPool;
	int meteorPoolIndex;
	

private://methods
	void createGameScreen();
	void createPools();
	void createActions();
	void stopGame();
	void increaseDifficulty();
	void resetGame();
	void resetHealth();
	void resetMeteor();
	void createUI();


public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	~GameLayer();

	void fallingObjectDone(Node* pSender);
	void animationDone(Node* pSender);
	void shockwaveDone();

	//game loop and touches
	virtual void update(float dt);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    // implement the "static create()" method manually
	CREATE_FUNC(GameLayer);
};

#endif // __GAME_LAYER_H__
