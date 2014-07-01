#include "Photon.h"
#include "Global.h"

int Photon::total = 20116;
cocos2d::Color3B Photon::colours[3] = {
    cocos2d::Color3B(255, 0, 0),
    cocos2d::Color3B(0, 255, 0),
    cocos2d::Color3B(0, 0, 255)
};

void Photon::setVelocity(float v, float directionAngle)
{
    _v = v;
    _direction = directionAngle;
}

void Photon::setColourValue(int val)
{
    _cval = val;
    _sprite->setOpacity(val);
}

int Photon::getColourValue()
{
    return _sprite->getOpacity();
}

void Photon::move(float dt)
{
    setPosition(getPosition() + cocos2d::Vec2(_v * sin(_direction), _v * cos(_direction)) * dt);
}

Photon *Photon::randomGen()
{
    float b_radius = RAND_BTW(onclr::photon_minradius, onclr::photon_maxradius);
    int b_colourval = RAND_BTW_INT(onclr::photon_mincolourval, onclr::photon_maxcolourval);
    int b_colour_idx = rand() % Photon::colour_ct;
    float b_hugtime = RAND_RATE(b_radius, 
        onclr::photon_minradius, onclr::photon_maxradius,
        onclr::photon_minhugtime, onclr::photon_maxhugtime);
    Photon *b = Photon::create(b_radius, Photon::colours[b_colour_idx]);
    b->setColourValue(b_colourval);
    b->setHugTime(b_hugtime);
    return b;
}
