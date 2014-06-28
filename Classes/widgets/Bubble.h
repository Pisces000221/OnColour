#ifndef __OnColour__Bubble_h__
#define __OnColour__Bubble_h__

#include "cocos2d.h"
#include "Global.h"

class Bubble : public cocos2d::Node
{
public:
    bool init(float radius, cocos2d::Color3B colour);
    static Bubble *create(float radius, cocos2d::Color3B colour)
    {
        Bubble *ret = new Bubble();
        ret->init(radius, colour);
        ret->autorelease();
        return ret;
    }
    void setColor(cocos2d::Color3B colour);

protected:
    float _radius;
    cocos2d::Sprite *_sprite;
};

#endif
