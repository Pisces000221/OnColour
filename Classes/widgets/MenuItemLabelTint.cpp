#include "MenuItemLabelTint.h"
#include "cocos2d.h"
using namespace cocos2d;

#define HALF_CLOSER_GREY(_c, _g) (_c.r + _g) >> 1, (_c.g + _g) >> 1, (_c.b + _g) >> 1

void MenuItemLabelTint::activate()
{
    if (_enabled) {
        this->unselected();
        MenuItem::activate();
    }
}

void MenuItemLabelTint::selected()
{
    if (_enabled) {
        MenuItem::selected();
        Action *runningAction = getActionByTag(TINT_ACTION_TAG);
        if (runningAction) _label->stopAction(runningAction);
        else _originalColour = _label->getColor();
        // Make the colour a half closer to grey (192, 192, 192)
        Action *action = TintTo::create(0.2f, HALF_CLOSER_GREY(_originalColour, 192));
        action->setTag(TINT_ACTION_TAG);
        _label->runAction(action);
    }
}

void MenuItemLabelTint::unselected()
{
    if (_enabled) {
        MenuItem::unselected();
        _label->stopActionByTag(TINT_ACTION_TAG);
        Action *action = TintTo::create(0.2f,
            _originalColour.r, _originalColour.g, _originalColour.b);
        action->setTag(TINT_ACTION_TAG);
        _label->runAction(action);
    }
}

void MenuItemLabelTint::setEnabled(bool enabled)
{
    if (_enabled ^ enabled)
        if (enabled)
            _label->setColor(_colorBackup);
        else {
            _colorBackup = this->getColor();
            _label->setColor(_disabledColor);
        }
    MenuItem::setEnabled(enabled);
}
