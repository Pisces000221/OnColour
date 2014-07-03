#ifndef __OnColour__TutorialScene_h__
#define __OnColour__TutorialScene_h__

#include "cocos2d.h"
#include "Global.h"

class Tutorial : public cocos2d::LayerColor
{
public:
    virtual bool init();  
    CREATE_FUNC(Tutorial);
    SCENE_FUNC(Tutorial);

    void goBack(cocos2d::Ref *sender);

protected:
    int _curPage, _totPages;
    std::vector<std::string> _texts;
    cocos2d::Sprite *_image;
    cocos2d::LayerColor *_textContainer;
    cocos2d::Label *_textLabel;
    bool _isMovingContainer;
    float _touchMoveDeltaY;

    void refreshDisp();
};

#endif
