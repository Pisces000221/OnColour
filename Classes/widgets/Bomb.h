#ifndef __OnColour__Bomb_h__
#define __OnColour__Bomb_h__

#include "cocos2d.h"
#include "Global.h"
#include "Photon.h"

// All we need is peace... I didn't mean to destroy you...
class Bomb : public Photon
{
public:
    static Bomb *create(float radius)
    {
        Bomb *ret = new Bomb();
        ret->init(radius);
        ret->_id = ++Photon::total;
        ret->autorelease();
        return ret;
    }
    virtual bool init(float radius);
    virtual bool isBomb() { return true; }

    static Bomb *randomGen();

protected:
};

#endif
