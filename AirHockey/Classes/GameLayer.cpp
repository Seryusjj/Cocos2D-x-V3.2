#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "AdmobHelper.h"

USING_NS_CC;
using namespace CocosDenshion;

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

GameLayer::GameLayer(){
    GameLayer::player1Score = 0;
    GameLayer::player2Score = 0;
    GameLayer::player1ScoreLabel = Label::createWithSystemFont("0","Arial",60);
    GameLayer::player2ScoreLabel = Label::createWithSystemFont("0","Arial",60);
    GameLayer::screenSize = Director::getInstance()->getVisibleSize();

    GameLayer::logo = Sprite::create("logo.png");
    GameLayer::player1 = GameSprite::create("mallet.png");
    GameLayer::player2 = GameSprite::create("mallet.png");
    GameLayer::ball = GameSprite::create("puck.png");
    GameLayer::players.pushBack(player1);
    GameLayer::players.pushBack(player2);

}

GameLayer::~GameLayer(){
    players.~Vector();
}



// on "init" you need to initialize your instance
bool GameLayer::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto background = Sprite::create("court.png");
    // position the nodes ond the layer
    background->setPosition(Point(screenSize.width*0.5 + origin.x, screenSize.height*0.5 + origin.y));
    logo->setPosition(Point(screenSize.width*0.5 + origin.x, screenSize.height*0.5 + origin.y));

    player1->setPosition(Point(screenSize.width-player1->radius()*2,screenSize.height*0.5));//right
    player1ScoreLabel->setPosition(Point(screenSize.width*0.5+60,screenSize.height-60));

    //left
    player2->setPosition(Point(origin.x+player2->radius()*2,screenSize.height*0.5));
    player2ScoreLabel->setPosition(Point(screenSize.width*0.5-60,screenSize.height-60));

    //center
    ball->setPosition(Point(screenSize.width*0.5,screenSize.height*0.5));
    // add the nodes as a child to this layer
    this->addChild(background, 0);
    this->addChild(ball, 1);
    this->addChild(player1,2);//player 1
    this->addChild(player2,2);//player 2
    this->addChild(player1ScoreLabel, 3);
    this->addChild(player2ScoreLabel, 3);
    this->addChild(logo,4);


    //activate touch input
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
    touchListener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
    touchListener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //start game loop
    this->schedule(schedule_selector(GameLayer::update));


    AdmobHelper::showAd();


    return true;
}

bool GameLayer::isPlayerCollidingWithBall(const float&  diffx,const float&  diffy){

    float distance1 = pow(diffx,2)+pow(diffy,2);
    return distance1<=getSquaredRadii();
}

///
/// \brief GameLayer::collisionsBetweenBallAndBorders
/// \param ballNextPosition
/// \param ballVector
/// \return true if goal.
///
bool GameLayer::collisionsBetweenBallAndBorders(Point& ballNextPosition,Point& ballVector){
    bool result = false;
    //height sides
    if(ballNextPosition.y<ball->radius()){
        ballNextPosition.y = ball->radius();
        ballVector.y*=-0.8f;
        SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }if(ballNextPosition.y>screenSize.height-ball->radius()){
        ballNextPosition.y = screenSize.height-ball->radius();
        ballVector.y*=-0.8f;
        SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }

    //check f collision or goal.
    //width sides
    if(ballNextPosition.x<ball->radius()){
        if(ballNextPosition.y < screenSize.height*0.5 - GOAL_WIDTH * 0.5
                || ballNextPosition.y > screenSize.height*0.5 + GOAL_WIDTH * 0.5 ){
            ballNextPosition.x = ball->radius();
            ballVector.x*=-0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }else{
            //goal
            ballNextPosition =  Point(screenSize.width*0.5,screenSize.height*0.5);
            ballVector = Point(0,0);
            result=true;
            player1Score++;
            SimpleAudioEngine::getInstance()->playEffect("score.wav");
        }

    }if(ballNextPosition.x>screenSize.width - ball->radius()){
        if(ballNextPosition.y<screenSize.height*0.5- GOAL_WIDTH * 0.5
                || ballNextPosition.y > screenSize.height*0.5 + GOAL_WIDTH * 0.5 ){
            ballNextPosition.x = screenSize.width - ball->radius();
            ballVector.x*=-0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }else{
            ballNextPosition =  Point(screenSize.width*0.5,screenSize.height*0.5);
            ballVector = Point(0,0);
            result=true;
            player2Score++;
            SimpleAudioEngine::getInstance()->playEffect("score.wav");
        }
    }
    return result;
}


void GameLayer::update(float delta){
    if(!logo->isVisible()){
        Point ballNextPosition = GameLayer::ball->getPosition();
        Point ballVector = GameLayer::ball ->getVector();

        //fuerza de rozamiento aplicada en cada loop
        ballVector.scale(0.98f);

        //aplicamos la fuerza a la posicion de la bola
        ballNextPosition.x += ballVector.x;
        ballNextPosition.y += ballVector.y;

        GameSprite* player;
        Point playerNextPosition;
        Point playerVector;


        for(int p = 0; p<players.size();p++){
            player = players.at(p);

            playerNextPosition = player->getPosition();
            playerVector = player->getVector();

            //distancia entre la bola y el jugador eje x
            float diffx = ball->getPositionX() - player->getPositionX();
            //distancia entre labola y e jugador eje y
            float diffy = ball->getPositionY() - player->getPositionY();
            if(isPlayerCollidingWithBall(diffx,diffy)){
                //calculamos la fuerza con la que se impulsara la bola
                float magBall = pow(ballVector.x,2)+pow(ballVector.y,2);
                float magPlayer = pow(playerVector.x,2)+pow(playerVector.y,2);

                //a partir de esa fuerza, aplicamos la raiz cuadrada porque
                //antes hicimos la potencia.

                float force = sqrt(magBall+magPlayer);
                //calculamos el angulo de aplicacion de la fuerza
                float angle = atan2(diffy,diffx);
                //coseno para el eje x (direccion en x)
                ballVector.x = force * cos(angle) ;
                //seno del angulo para el eje y (direccion en y)
                ballVector.y = force * sin(angle) ;
                //sumamos a la posicion del player, su radio y el de la bola
                //y aplicamos seno o coseno del angulo para si es + o -
                ballNextPosition.x = playerNextPosition.x + (player->radius()+ball->radius()+force) * cos(angle);
                ballNextPosition.y = playerNextPosition.y + (player->radius()+ball->radius()+force) * sin(angle);

                SimpleAudioEngine::getInstance()->playEffect("hit.wav");
            }


        }

        //collsion between ball and screen sides and check if goal occur
        if(collisionsBetweenBallAndBorders(ballNextPosition,ballVector)){
            player1ScoreLabel->setString(std::to_string(player1Score).c_str());
            player2ScoreLabel->setString(std::to_string(player2Score).c_str());

            //update player position
            player1->setPosition(Point(screenSize.width-player1->radius()*2,screenSize.height*0.5));//right
            player2->setPosition(Point(player2->radius()*2,screenSize.height*0.5));
            //clear players constraints
            player1->setTouch(Touch());
            player1->setVector(Point(0,0));
            player2->setTouch(Touch());
            player2->setVector(Point(0,0));
            //reactivate the logo
            logo->setVisible(true);
            //show adds
            AdmobHelper::showAd();
        }
        //update ball position

        ball->setVector(ballVector);
        ball -> setPosition(ballNextPosition);
    }
}

void GameLayer::checkPlayerLimits(Point& nextPosition,const GameSprite* const player){
    //screen limits
    if(nextPosition.x<player->radius()){
        nextPosition.x = player->radius();
    }if(nextPosition.x>screenSize.width-player->radius()){
        nextPosition.x = screenSize.width-player->radius();
    }if(nextPosition.y<player->radius()){
        nextPosition.y = player->radius();
    }if(nextPosition.y>screenSize.height-player->radius()){
        nextPosition.y = screenSize.height-player->radius();
    }

    //cour limits
    //left cour
    if(player->getPositionX()<screenSize.width*0.5){
        if(nextPosition.x>=screenSize.width*0.5 - player->radius())
        {
            nextPosition.x = screenSize.width*0.5- player->radius();
        }
    }
    //right cour
    if(player->getPositionX()>screenSize.width*0.5){
        if(nextPosition.x<=screenSize.width*0.5 + player->radius())
        {
            nextPosition.x = screenSize.width*0.5 + player->radius();
        }
    }

}

void GameLayer::onTouchesBegan(const std::vector<Touch *> &touches, Event *unused_event){
    if(logo->isVisible()){
        logo->setVisible(false);
        AdmobHelper::hideAd();
    }else{

        Touch* touch = nullptr;
        Point tap;
        for(uint i=0;i<touches.size();i++){
            touch=touches.at(i);
            if(touch != nullptr){
                tap=touch->getLocation();
                for(int p=0;p<players.size();p++){
                    if(players.at(p)->getBoundingBox().containsPoint(tap)){
                        players.at(p)->setTouch(*touch);
                    }
                }
            }
        }
    }
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event){
    Touch* touch = nullptr;
    Point tap;
    for(uint i=0;i<touches.size();i++){
        touch=touches.at(i);
        if(touch!=nullptr){
            tap=touch->getLocation();
            for(int p=0;p<players.size();p++){
                auto player = players.at(p);
                if(player->getTouch()._ID==touch->_ID){
                    Point nextPosition = tap;
                    checkPlayerLimits(nextPosition,player);
                    Point vector = Point(nextPosition.x-player->getPositionX(),
                                         nextPosition.y-player->getPositionY());
                    player->setVector(vector);
                    player->setPosition(nextPosition);
                }
            }
        }
    }
}


void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event){
    Touch* touch=nullptr;
    Point tap;
    for(uint i=0;i<touches.size();i++){
        touch=touches.at(i);
        if(touch!=nullptr){
            tap=touch->getLocation();
            for(int p=0;p<players.size();p++){
                if(players.at(p)->getTouch()._ID==touch->_ID){
                    players.at(p)->setTouch(Touch());
                    players.at(p)->setVector(Point(0,0));
                }
            }
        }
    }
}



