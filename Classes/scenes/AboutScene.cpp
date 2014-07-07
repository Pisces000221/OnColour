#include "AboutScene.h"
#include "Global.h"
using namespace cocos2d;

#define BOTOOM_LEFT_CORNER(_node) (_node->getPosition() - _node->getAnchorPointInPoints() * _node->getScale())
#define BOTOOM_RIGHT_CORNER(_node) (BOTOOM_LEFT_CORNER(_node) + cocos2d::Vec2(_node->getContentSize().width * _node->getScale(), 0))
#define FADE_IN_ELEM(_node, _delay) do { _node->setOpacity(0); _node->runAction(FADE_IN_DELAY(_delay, 0.3)); } while (0)

bool About::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;
    float s_ratio = (onclr::ratio - 1) * 0.7 + 1;
    CREATE_GO_BACK_MENUITEM(About, backItem);
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1000);

    auto title = onclr::label("About", 48 * s_ratio, true);
    title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    title->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    title->setColor(Color3B::BLACK);
    title->setOpacity(0);
    this->addChild(title, 1000);
    title->runAction(FADE_IN_DELAY(0.8, 0.3));

    // Support scrolling
    auto scroll = Layer::create();
    this->addChild(scroll);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, scroll](Touch *touch, Event *event) {
        _dragStartPos = scroll->getPositionY();
        return true;
    };
    listener->onTouchMoved = [this, scroll](Touch *touch, Event *event) {
        float y = _dragStartPos + touch->getLocation().y - touch->getStartLocation().y;
        scroll->setPositionY(y);
    };
    listener->onTouchEnded = [this, scroll](Touch *touch, Event *event) {
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Create contents
    auto logo = Sprite::create("images/cocos2dx_portrait.png");
    logo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    logo->setPosition(Vec2(4, onclr::vsize.height - 48 * s_ratio - 4));
    logo->setScale(0.6180339887 * onclr::vsize.height / logo->getContentSize().height);
    FADE_IN_ELEM(logo, 0.9);
    scroll->addChild(logo);
    auto logo_lbl = onclr::label("Game Engine: Cocos2d-x", 24 * s_ratio, false, Color3B::BLACK);
    logo_lbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    logo_lbl->setPosition(BOTOOM_LEFT_CORNER(logo));
    FADE_IN_ELEM(logo_lbl, 1);
    scroll->addChild(logo_lbl);

    auto dev = Sprite::create("images/dev.png");
    dev->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    dev->setPosition(Vec2(onclr::vsize.width - 4, onclr::vsize.height - 48 * s_ratio - 4));
    dev->setScale(0.3819660113 * onclr::vsize.width / dev->getContentSize().width);
    FADE_IN_ELEM(dev, 1.2);
    scroll->addChild(dev);
    auto dev_lbl = onclr::label("Pisces000221 the developer", 24 * s_ratio, false, Color3B::BLACK);
    dev_lbl->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    dev_lbl->setPosition(BOTOOM_RIGHT_CORNER(dev));
    FADE_IN_ELEM(dev_lbl, 1.3);
    scroll->addChild(dev_lbl);

    return true;
}

void About::goBack(Ref *sender)
{
    GO_BACK_ANIMATED;
}
