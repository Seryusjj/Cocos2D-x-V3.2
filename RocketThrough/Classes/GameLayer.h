#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "rocketthrough/Rocket.h"
#include "rocketthrough/LineContainer.h"


USING_NS_CC;
using namespace CocosDenshion;

enum {
    background,
    middleground,
    foreground
};

enum {
    spriteRocket,
    spritePlanet,
    spriteBoost,
    spriteStar
};

typedef enum gamestates {
    gameIntro,
    gamePaused,
    gamePlay,
    gameOver
    
}GameState;




class GameLayer : public Layer {
    
	
    Rocket * _rocket;
    LineContainer * _lineContainer;
    
    SpriteBatchNode *_gameBatchNode;
    Label * _scoreDisplay;
    
    GameSprite * _pauseBtn;
    Sprite * _intro;
    Sprite *_gameOver;
    Sprite *_paused;
    
    ParticleSystem * _star;
    ParticleSystem * _jet;
    ParticleSystem * _boom;
    ParticleSystem * _comet;
    ParticleSystem * _pickup;
    ParticleSystem * _warp;
    
    Vector<Node*>  _planets;
    Size _screenSize;
    
    GameState _state;
    
    bool _drawing;
    bool _running;

    std::vector<Point> _grid;
    int _gridIndex;
    
    int _minLineLength;
    float _cometInterval;
    float _cometTimer;
    
    void resetGame();
    void resetStar();
    void killPlayer();
    
    void createGameScreen();
    void createParticles();
    void createStarGrid();

	int _score;
	float _timeBetweenPickups;
    
public:
	~GameLayer();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    
    void update (float dt);
	
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
	

};

#endif // __GAMELAYER_H__
