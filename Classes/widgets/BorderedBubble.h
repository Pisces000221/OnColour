#ifndef __OnColour__BorderedBubble_h__
#define __OnColour__BorderedBubble_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"

class BorderedBubble : public Bubble
{
public:
    bool init(float radius, float borderwidth, cocos2d::Color4F colour);
    static BorderedBubble *create(float radius, float borderwidth, cocos2d::Color4F colour)
    {
        BorderedBubble *ret = new BorderedBubble();
        ret->init(radius, borderwidth, colour);
        ret->autorelease();
        return ret;
    }
    void setBorderProgress(float prog); // prog in [0, 100]

protected:
    cocos2d::ProgressTimer *_border;
};

#endif
