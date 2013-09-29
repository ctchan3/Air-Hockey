//
//  GameSprite.h
//  Air Hockey
//
//  Created by To Chan on 15/9/13.
//
//

#ifndef __Air_Hockey__GameSprite__
#define __Air_Hockey__GameSprite__

#include "cocos2d.h"

using namespace cocos2d;

class GameSprite : public CCSprite {
public:
    CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
    CC_SYNTHESIZE(CCPoint, _vector, Vector);
    CC_SYNTHESIZE(CCTouch *, _touch, Touch);
    
    GameSprite(void);
    ~GameSprite(void);
    
    static GameSprite* gameSpriteWithFile(const char * pszFileName);
    virtual void setPosition(const CCPoint& pos);
    float radius();
};

#endif /* defined(__Air_Hockey__GameSprite__) */
