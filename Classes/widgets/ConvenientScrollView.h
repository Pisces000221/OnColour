#include "Global.h"

#define TURN_INTO_SCROLL_VIEW(__t__, __layer__, __height__, __visheight__) do { \
    auto listener = cocos2d::EventListenerTouchOneByOne::create(); \
    float maxpos = ((__height__) - (__visheight__)); \
    listener->setSwallowTouches(false); \
    listener->onTouchBegan = [this, __layer__](cocos2d::Touch *touch, cocos2d::Event *event) { \
        __t__ = __layer__->getPositionY(); \
        return true; \
    }; \
    listener->onTouchMoved = [this, __layer__, maxpos](cocos2d::Touch *touch, cocos2d::Event *event) { \
        float y = __t__ + touch->getLocation().y - touch->getStartLocation().y; \
        FIX_POS(y, 0, maxpos); \
        __layer__->setPositionY(y); \
    }; \
    listener->onTouchEnded = [](cocos2d::Touch *touch, cocos2d::Event *event) { \
    }; \
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, __layer__); \
} while (0)
