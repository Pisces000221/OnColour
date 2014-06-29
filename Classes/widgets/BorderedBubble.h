#ifndef __OnColour__BorderedBubble_h__
#define __OnColour__BorderedBubble_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"

class BorderedBubble : public Bubble
{
public:
    bool init(float radius, float borderwidth, cocos2d::Color3B colour);
    static BorderedBubble *create(float radius, float borderwidth, cocos2d::Color3B colour)
    {
        BorderedBubble *ret = new BorderedBubble();
        ret->init(radius, borderwidth, colour);
        ret->autorelease();
        return ret;
    }
    void setBorderProgress(float prog); // prog in [0, 1]
    void reset(float anim_time);

protected:
    cocos2d::ProgressTimer *_border;
    float _animPastTime;
};

#endif
