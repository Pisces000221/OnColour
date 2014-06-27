#ifndef __OnColour__Bubble_h__
#define __OnColour__Bubble_h__

#include "cocos2d.h"
#include "Global.h"

class Bubble : public cocos2d::DrawNode
{
public:
    bool init(float radius, cocos2d::Color4F colour);
    static Bubble *create(float radius, cocos2d::Color4F colour)
    {
        Bubble *ret = new Bubble();
        ret->init(radius, colour);
        ret->autorelease();
        return ret;
    }
};

#endif
