#ifndef __OnColour__MenuItemLabelTint_h__
#define __OnColour__MenuItemLabelTint_h__

#include "cocos2d.h"

class MenuItemLabelTint : public cocos2d::MenuItemLabel
{
public:
    // Overrides
    virtual void activate() override;
    virtual void selected() override;
    virtual void unselected() override;
    virtual void setEnabled(bool enabled) override;

    static MenuItemLabelTint *create(cocos2d::Node *label, const cocos2d::ccMenuCallback &callback)
    {
        MenuItemLabelTint *ret = new MenuItemLabelTint();
        ret->initWithLabel(label, callback);
        ret->autorelease();
        return ret;
    }
protected:
    static const int TINT_ACTION_TAG = 147106;
    cocos2d::Color3B _originalColour;
};

#endif
