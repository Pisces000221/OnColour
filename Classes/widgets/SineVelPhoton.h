#ifndef __OnColour__SineVelPhoton_h__
#define __OnColour__SineVelPhoton_h__

#include "cocos2d.h"
#include "Global.h"
#include "Photon.h"

class SineVelPhoton : public Photon
{
public:
    static SineVelPhoton *create(float radius, cocos2d::Color3B colour)
    {
        SineVelPhoton *ret = new SineVelPhoton();
        ret->init(radius, colour);
        ret->_id = ++Photon::total;
        ret->autorelease();
        return ret;
    }
    virtual bool init(float radius, cocos2d::Color3B colour);
    void setPeriod(float period) { _period = period; }
    void setAmplitude(float amplitude) { _amplitude = amplitude; }
    virtual void move(float dt);

    static const int sin_colour_ct = 3;
    static cocos2d::Color3B sin_colours[3];
    static SineVelPhoton *randomGen();

protected:
    float _period, _amplitude;
};

#endif
