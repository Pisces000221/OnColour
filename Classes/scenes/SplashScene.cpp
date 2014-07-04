#include "SplashScene.h"
#include "StartupScene.h"
#include "Global.h"
using namespace cocos2d;

bool Splash::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;

    auto sprite = Sprite::create("images/cocos2dx_portrait.png");
    sprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprite->setPosition(Vec2(-24, onclr::vsize.height + 48));
    sprite->setOpacity(144);
    sprite->setScale(onclr::vsize.height * 0.9 / sprite->getTextureRect().size.width);
    this->addChild(sprite);

    auto label_1 = onclr::label("Powered by", 35, false);
    label_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    label_1->setPosition(Vec2(onclr::vsize.width * 0.65, 55));
    label_1->setColor(Color3B(0, 0, 0));
    this->addChild(label_1);

    auto label_2 = onclr::label("Cocos2d-x", 52, true);
    label_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    label_2->setPosition(Vec2(onclr::vsize.width * 0.65, 3));
    label_2->setColor(Color3B(0, 0, 0));
    this->addChild(label_2);

    // Fade in on startup
    auto cover = LayerColor::create(Color4B::BLACK);
    this->addChild(cover, 1024);
    cover->runAction(FadeOut::create(0.15f));

    this->scheduleOnce(schedule_selector(Splash::goOn), 2);

    return true;
}

void Splash::goOn(float dt)
{
    auto nextScene = Startup::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.8, nextScene));
}
