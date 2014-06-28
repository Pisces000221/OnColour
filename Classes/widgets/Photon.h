#ifndef __OnColour__Photon_h__
#define __OnColour__Photon_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"

class Photon : public Bubble
{
public:
    static Photon *create(float radius, cocos2d::Color3B colour)
    {
        Photon *ret = new Photon();
        ret->init(radius, colour);
        ret->autorelease();
        return ret;
    }
    // @params directionAngle The angle, in a nutshell, arctan(k).
    void setVelocity(float v, float directionAngle);
    void setColourValue(int val);
    void move(float dt);

protected:
    float _v, _direction;
    int _cval;
};

#endif
