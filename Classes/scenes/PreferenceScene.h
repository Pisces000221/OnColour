#ifndef __OnColour__PreferenceScene_h__
#define __OnColour__PreferenceScene_h__

#include "cocos2d.h"
#include "Global.h"

class PreferenceLayer : public cocos2d::LayerColor
{
public:
    virtual bool init();  
    CREATE_FUNC(PreferenceLayer);
    SCENE_FUNC(PreferenceLayer);

    void goBack(cocos2d::Ref *sender);
};

#endif
