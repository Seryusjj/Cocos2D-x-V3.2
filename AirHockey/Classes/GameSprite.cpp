#include "GameSprite.h"

GameSprite::GameSprite(){
    getTouch().setTouchInfo(-1,0,0);
    _vector = Point(0,0);
}

GameSprite::~GameSprite(){

}


GameSprite* GameSprite::create(const std::string& filename){
    GameSprite *sprite = new (std::nothrow) GameSprite();
    if (sprite && sprite->initWithFile(filename)){
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}



