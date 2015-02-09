#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Box2D/Box2D.h"
#include "CollisionListener.h"
#include "GLES-Render.h"
#include "LineContainer.h"
#include "Ball.h"
#include "Cue.h"


#define PTM_RATIO 32

USING_NS_CC;


enum {
	kBackground,
	kMiddleground,
	kForeground
};

enum {
	kGameIntro,
	kGamePlay,
	kGameOver
};

class GameLayer : public Layer
{
private:
	GLESDebugDraw * _debugDraw;
	CollisionListener * _collisionListener;

	Touch * _touch;
	Vector<Sprite*> _balls;
	Vector<Sprite*> _pockets;
	SpriteBatchNode * _gameBatchNode;
	Sprite * _intro;
	Sprite * _gameOver;
	Label * _timer;
	LineContainer * _lineContainer;
	Point _cueStartPoint;
	Size _screenSize;
	bool _running;
	bool _usingCue;
	float _pullBack;
	float _cueAngle;
	int _gameState;
	int _ballsInPlay;
	int _time;

	Ball * _player;
	Cue * _cue;

	void placeCue(Point position);
	void resetGame();

public:
	~GameLayer();
	GameLayer();
	
	CC_SYNTHESIZE(b2World *, _world, World);
	CC_SYNTHESIZE(bool, _canShoot, CanShoot);

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene();

	void initPhysics();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	//touch call back
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//update method
	void update(float dt);
	void ticktock();


	// implement the "static create()" method manually
	CREATE_FUNC(GameLayer);
};

#endif // __HELLOWORLD_SCENE_H__
