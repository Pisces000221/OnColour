#ifndef __OnColour__ToggleBubble_h__
#define __OnColour__ToggleBubble_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"

class ToggleBubble : public Bubble
{
public:
    ToggleBubble() : _on(false), _lastTouchIsInside(false) {}
    bool init(float radius, cocos2d::Color3B colour, const cocos2d::ccMenuCallback &callback);
    static ToggleBubble *create(float radius, cocos2d::Color3B colour, const cocos2d::ccMenuCallback &callback)
    {
        ToggleBubble *ret = new ToggleBubble();
        ret->init(radius, colour, callback);
        ret->autorelease();
        return ret;
    }
    void setOn(bool on);
    bool isOn() { return _on; }
    virtual void setColor(cocos2d::Color3B colour);

protected:
    cocos2d::Color3B _initialColour;
    float _initialScale;
    bool _on;
    bool _lastTouchIsInside;
    cocos2d::ccMenuCallback _call;
};

#endif
