#ifndef __FAMESPRITE_H__
#define __FAMESPRITE_H__
#include "cocos2d.h"

USING_NS_CC;

class GameSprite : public Sprite{


    //the line above generates:
    //protected: Point _nextPosition;
    //public: virtual Point getNextPosition() const { return _nextPosition; }
    //public: virtual void setNextPosition(Point var){ _nextPosition = var; }
    CC_SYNTHESIZE(Point,_vector,Vector)
    CC_SYNTHESIZE(Touch,_touch,Touch)

public:
    GameSprite();
    ~GameSprite();

    //lo que se implementa en el *.h es automaticamente inline
    float radius() const {return getTexture()->getContentSize().width*0.5f;}
    static GameSprite* create(const std::string& filename);



};

#endif // __FAMESPRITE_H__
