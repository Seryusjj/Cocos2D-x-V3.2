#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#define GOAL_WIDTH 400 //width in pixels for the goals
#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

class GameLayer : public Layer
{

private:
    Sprite* logo;
    GameSprite* player1;
    GameSprite* player2;
    GameSprite* ball;
    Vector<GameSprite*> players;
    Label* player1ScoreLabel;
    Label* player2ScoreLabel;
    Size screenSize;
    int player1Score,player2Score;
    void playerScore(int player);


public:
    GameLayer();
    ~GameLayer();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    virtual void onTouchesBegan(const std::vector<Touch *> &touches, Event *unused_event);

    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

    virtual void update(float delta);

    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer)
private:
    void checkPlayerLimits(Point& nextPosition,const GameSprite* const player);
    bool isPlayerCollidingWithBall(const float&  diffx,const float&  diffy);
    bool collisionsBetweenBallAndBorders(Point& ballNextPosition,Point& ballVector);
    float getSquaredRadii()const{return  pow(player1->radius()+ball->radius(),2); }

};

#endif // __GAMELAYER_H__
