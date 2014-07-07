#ifndef __OnColour__AboutScene_h__
#define __OnColour__AboutScene_h__

#include "cocos2d.h"
#include "Global.h"

class About : public cocos2d::LayerColor
{
public:
    virtual bool init();  
    CREATE_FUNC(About);
    SCENE_FUNC(About);

    void goBack(cocos2d::Ref *sender);

protected:
    float _dragStartPos;
};

#endif
