#ifndef __OnColour__GameScene_h__
#define __OnColour__GameScene_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"

class Gameplay : public cocos2d::LayerColor
{
public:
    virtual bool init();  
    CREATE_FUNC(Gameplay);
    SCENE_FUNC(Gameplay);

protected:
    Bubble *_player;
    cocos2d::EventKeyboard::KeyCode pressedKeys[2];

    void goBack(cocos2d::Ref *sender);
    void moveBall(float acc_x, float acc_y, float dt);
};

#endif
