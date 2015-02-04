#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SimpleAudioEngine.h"
#include "rocketthrough/Rocket.h"
#include "rocketthrough/LineContainer.h"

USING_NS_CC;

using namespace CocosDenshion;


enum {
	kBackground,
	kMiddleground,
	kForeground
};

enum {
	kSpriteRocket,
	kSpritePlanet,
	kSpriteBoost,
	kSpriteStar
};

typedef enum gamestates {
	kGameIntro,
	kGamePaused,
	kGamePlay,
	kGameOver

}GameState;


class GameLayer : public Layer {


	Rocket * _rocket;
	LineContainer * _lineContainer;

	SpriteBatchNode *_gameBatchNode;
	Label * _scoreDisplay;

	Sprite * _pauseBtn;
	Sprite * _intro;
	Sprite *_gameOver;
	Sprite *_paused;

	ParticleSystem * _star;
	ParticleSystem * _jet;
	ParticleSystem * _boom;
	ParticleSystem * _comet;
	ParticleSystem * _pickup;
	ParticleSystem * _warp;

	Vector<GameSprite *> _planets;
	Size _screenSize;

	GameState _state;

	bool _drawing;
	bool _running;

	std::vector<Point> _grid;
	int _gridIndex;

	int _minLineLength;
	float _cometInterval;
	float _cometTimer;

	void resetGame(void);
	void resetStar(void);
	void killPlayer(void);

	void createGameScreen(void);
	void createParticles(void);
	void createStarGrid(void);

	int _score;
	float _timeBetweenPickups;

public:
	~GameLayer(void);

	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();

	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static Scene* createScene();

	//main loop
	void update(float dt);


	//touch call back
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(GameLayer);


};

#endif // __GAMELAYER_H__
