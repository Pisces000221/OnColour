#ifndef __OnColour__SplashScene_h__
#define __OnColour__SplashScene_h__

#include "cocos2d.h"
#include "Global.h"

class Splash : public cocos2d::LayerColor
{
public:
    virtual bool init();  
    CREATE_FUNC(Splash);
    SCENE_FUNC(Splash);

    void goOn(float dt);
};

#endif
