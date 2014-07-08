#include "AboutScene.h"
#include "Global.h"
#include "ConvenientScrollView.h"
using namespace cocos2d;

#define BOTTOM_LEFT_CORNER(_node) (_node->getPosition() - _node->getAnchorPointInPoints() * _node->getScale())
#define BOTTOM_RIGHT_CORNER(_node) (BOTTOM_LEFT_CORNER(_node) + cocos2d::Vec2(_node->getContentSize().width * _node->getScale(), 0))
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

    // Create a container, and turn it into a scroll view later
    auto scroll = Layer::create();
    this->addChild(scroll);

    // Create contents
    auto logo = Sprite::create("images/cocos2dx_portrait.png");
    logo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    logo->setPosition(Vec2(4, onclr::vsize.height - 48 * s_ratio - 4));
    logo->setScale(0.6180339887 * onclr::vsize.height / logo->getContentSize().height);
    FADE_IN_ELEM(logo, 0.9);
    scroll->addChild(logo);
    auto logo_lbl = onclr::label("Game Engine: Cocos2d-x", 24 * s_ratio, false, Color3B::BLACK);
    logo_lbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    logo_lbl->setPosition(BOTTOM_LEFT_CORNER(logo));
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
    dev_lbl->setPosition(BOTTOM_RIGHT_CORNER(dev));
    FADE_IN_ELEM(dev_lbl, 1.3);
    scroll->addChild(dev_lbl);

    auto popfx_lbl = onclr::label("Sound FX by\nJacob McCrary on FreeSound.org",
        30 * s_ratio, true, Color3B::BLACK, TextHAlignment::RIGHT);
    popfx_lbl->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    popfx_lbl->setPosition(BOTTOM_RIGHT_CORNER(dev_lbl) - Vec2(12, 12));
    FADE_IN_ELEM(popfx_lbl, 1.5);
    scroll->addChild(popfx_lbl);

    auto ccbysa = Sprite::create("images/cc_by_sa.png");
    ccbysa->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    ccbysa->setPosition(Vec2(onclr::vsize.width * 0.5, BOTTOM_LEFT_CORNER(popfx_lbl).y - popfx_lbl->getContentSize().height - 30));
    ccbysa->setScale(0.4 * onclr::vsize.width / ccbysa->getContentSize().width);
    FADE_IN_ELEM(ccbysa, 1.7);
    scroll->addChild(ccbysa);
    auto ccbysa_lbl = onclr::label(
        "All artworks & sounds are released under\nCC-BY-SA International 4.0",
        26 * s_ratio, false, GREY_3B(48), TextHAlignment::CENTER);
    ccbysa_lbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    ccbysa_lbl->setPosition(Vec2(onclr::vsize.width * 0.5, BOTTOM_LEFT_CORNER(ccbysa).y));
    FADE_IN_ELEM(ccbysa_lbl, 1.8);
    scroll->addChild(ccbysa_lbl);

    // Support scrolling
    float miny = BOTTOM_LEFT_CORNER(ccbysa_lbl).y - ccbysa_lbl->getContentSize().height;
    TURN_INTO_SCROLL_VIEW(_dragStartPos, scroll, _contentSize.height - miny, _contentSize.height);

    return true;
}

void About::goBack(Ref *sender)
{
    GO_BACK_ANIMATED;
}
