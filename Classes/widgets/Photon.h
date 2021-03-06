#ifndef __OnColour__Photon_h__
#define __OnColour__Photon_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"

//class Bubble;
class Photon : public Bubble
{
public:
    Photon();
    static Photon *create(float radius, cocos2d::Color3B colour)
    {
        Photon *ret = new Photon();
        ret->init(radius, colour);
        ret->_id = ++total;
        ret->autorelease();
        return ret;
    }
    int getID() { return _id; }
    // Call this when created me
    void initStartPos();
    // @params directionAngle The angle, in a nutshell, arctan(k).
    void setVelocity(float v, float directionAngle);
    void setColourValue(int val);
    int getColourValue();
    void setHugTime(float hugTime) { _hugTime = hugTime; }
    float getHugTime() { return _hugTime; }
    virtual void move(float dt);
    virtual bool isBomb() { return false; }

    static const int colour_ct = 3;
    static /*const*/ cocos2d::Color3B colours[3];
    static Photon *randomGen();

protected:
    float _v, _direction;
    cocos2d::Vec2 _startPos;
    float _moveTime;
    float _hugTime;
    int _cval, _id;
    static int total;
};

#endif
