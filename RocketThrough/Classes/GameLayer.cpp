/*
 
 Background music:
 8bit Dungeon Level by Kevin MacLeod (incompetech.com)
 
 */

#include "GameLayer.h"



USING_NS_CC;

using namespace CocosDenshion;


GameLayer::~GameLayer () {
    
	_planets.~Vector();

}

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();
    
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
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	//init game values
	_screenSize = CCDirector::sharedDirector()->getWinSize();
    _drawing = false;
    _minLineLength = _screenSize.width * 0.07f;
    _state = gameIntro;
    
    createGameScreen();
    
    createParticles();
    
    createStarGrid();
    
    //listen for touches
    this->setTouchEnabled(true);
    
    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
    
    
    
    return true;
}

void GameLayer::update (float dt) {
    
    
}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){}
void GameLayer::onTouchMoved(Touch *touch, Event *unused_event){}


void GameLayer::onTouchEnded(Touch *touch, Event *unused_event) {
    
    if (_state == gameIntro) {
        
        _intro->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = gamePlay;
        //resetGame();
        return;
        
    } else if (_state == gameOver) {
        
        _gameOver->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = gamePlay;
        //resetGame();
        return;
        
    } else if (_state == gamePaused) {
        
        _pauseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("btn_pause_off.png"));
        _paused->setVisible(false);
        _state = gamePlay;
        _running = true;
        return;
    }
    
    if (!_running) return;
    
    

}

void GameLayer::resetGame () {

    _rocket->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.1f));
    _rocket->setOpacity(255);
    _rocket->setVisible(true);
    _rocket->reset();
    
    _cometInterval = 4;
    _cometTimer = 0;
    _timeBetweenPickups = 0.0;
    
    _score = 0;
    char szValue[100] = {0};
    sprintf(szValue, "%i", _score);
    _scoreDisplay->setString(szValue);
    
    _lineContainer->reset();
    
    //shuffle grid cells
    std::random_shuffle(_grid.begin(), _grid.end());
    _gridIndex = 0;
    
    resetStar();
    
    _warp->stopSystem();
    
    _running = true;
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect("rocket.wav", true);
    
}

void GameLayer::resetStar() {
   
}

void GameLayer::killPlayer() {



}


void GameLayer::createGameScreen () {
    
    Sprite * bg = Sprite::create("bg.png");
    bg->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, background);
    
    
    _lineContainer = LineContainer::create();
    this->addChild(_lineContainer);
    
    
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = SpriteBatchNode::create("sprite_sheet.png", 100);

    this->addChild(_gameBatchNode, foreground);
    
    _rocket = Rocket::create();
    _rocket->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.1f));
    _gameBatchNode->addChild(_rocket, foreground, spriteRocket);
    

    //add planets
    GameSprite * planet;
	std::string spriteName = "";
	for (int i = 1; i < 8; i++){
		spriteName = "planet_";
		spriteName.append(std::to_string(i));
		spriteName.append(".png");
		planet = GameSprite::createWithFrameName(spriteName.c_str);
		planet->setPosition(Point(_screenSize.width * 0.25f, _screenSize.height * 0.8f));
		_gameBatchNode->addChild(planet, background, spritePlanet);
		_planets.pushBack(planet);
		spriteName = "";
	}
    
    
    
    
    Sprite * scoreLabel = Sprite::createWithSpriteFrameName("label_score.png");
    scoreLabel->setPosition(Point(_screenSize.width * 0.4f, _screenSize.height * 0.95));
    _gameBatchNode->addChild(scoreLabel, background);

	_scoreDisplay = Label::createWithBMFont("font.fnt", "0", TextHAlignment::LEFT, _screenSize.width * 0.5f);
    _scoreDisplay->setAnchorPoint(Point(0,0.5));
	_scoreDisplay->setPosition(Point(_screenSize.width * 0.48f, _screenSize.height * 0.95f));
    this->addChild(_scoreDisplay, background);
    
    _pauseBtn = GameSprite::createWithFrameName("btn_pause_off.png");
    _pauseBtn->setVisible(false);
    _pauseBtn->setPosition(ccp(_screenSize.width * 0.06f, _screenSize.height * 0.95f));
    _gameBatchNode->addChild(_pauseBtn, background);
    
    
    _intro = Sprite::createWithSpriteFrameName("logo.png");
	_intro->setPosition(Point(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    Sprite *play = Sprite::createWithSpriteFrameName("label_play.png");
    play->setPosition(ccp(_intro->boundingBox().size.width * 0.5f, -_intro->boundingBox().size.height * 0.5f));
    _intro->addChild(play);
    _gameBatchNode->addChild(_intro, foreground);
    
    _gameOver = Sprite::createWithSpriteFrameName("gameOver.png");
    _gameOver->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _gameOver->setVisible(false);
    _gameBatchNode->addChild(_gameOver, foreground);
    
    _paused = Sprite::createWithSpriteFrameName("label_paused.png");
    _paused->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _paused->setVisible(false);
    _gameBatchNode->addChild(_paused, foreground);
}


void GameLayer::createParticles() {

    

}

void GameLayer::createStarGrid() {
    //create grid
    float gridFrame = _screenSize.width * 0.1f;
    int tile = 32;
    int rows = (_screenSize.height - 4 * gridFrame)/tile;
    int cols = (_screenSize.width  - 2 * gridFrame)/tile;
    
	int count = _planets.size();
    GameSprite * planet;
    Point cell;
    bool overlaps;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cell = ccp(gridFrame + c * tile, 2 * gridFrame + r * tile);
            overlaps = false;
            for (int j = 0; j < count; j++) {
                planet = (GameSprite *) _planets.at(j);
                if (pow(planet->getPositionX() - cell.x, 2) + pow(planet->getPositionY() - cell.y, 2) <= pow(planet->getRadius() * 1.2f, 2)) {
                    overlaps = true;
                }
            }
            if (!overlaps) _grid.push_back(cell);
        }
    }
    CCLOG("POSSIBLE STARS: %i", _grid.size());
    
}