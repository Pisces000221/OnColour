#ifndef __OnColour__GameScene_h__
#define __OnColour__GameScene_h__

#include "cocos2d.h"
#include "Global.h"

class Gameplay : public cocos2d::LayerColor
{
public:
    virtual bool init();  
    CREATE_FUNC(Gameplay);
    SCENE_FUNC(Gameplay);

protected:
    void goBack(cocos2d::Ref *sender);
};

#endif
