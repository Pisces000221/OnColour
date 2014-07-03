#include "TutorialScene.h"
#include "Global.h"
using namespace cocos2d;

bool Tutorial::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;
    float s_ratio = (onclr::ratio - 1) * 0.7 + 1;
    CREATE_GO_BACK_MENUITEM(Tutorial, backItem);
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1000);

    auto title = onclr::label("How to play", 48 * s_ratio, true);
    title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    title->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    title->setColor(Color3B::BLACK);
    title->setOpacity(0);
    this->addChild(title, 1000);
    title->runAction(FADE_IN_DELAY(0.8, 0.3));

    // Reset page info
    _curPage = 0;
    // Read page info
    std::string textFileName =
        FileUtils::getInstance()->getWritablePath() + "tutorial.txt";
    CCLOG("file name: %s", textFileName.c_str());
    Data data = FileUtils::getInstance()->getDataFromFile("tutorial/tutorial.txt");
    FILE *fp = fopen(textFileName.c_str(), "w");
    fwrite(data.getBytes(), 1, data.getSize(), fp);
    fclose(fp);
    fp = fopen(textFileName.c_str(), "r");
    std::string s = "";
    while (!feof(fp)) {
        char line[1024];
        fgets(line, 1024, fp);
        if (!strcmp(line, "#\n")) { _texts.push_back(s); s = ""; }
        else if (!strcmp(line, "##\n")) { _texts.push_back(s); s = ""; break; }
        else s += line;
    }
    fclose(fp);
    _totPages = _texts.size();
    CCLOG("Tutorial: %d page(s) in total", _totPages);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch *touch, Event *event) {
        Vec2 p = touch->getLocation();
        if (p.x > _contentSize.width * 1.0/3.0 &&
         p.x < _contentSize.width * 2.0/3.0) {
            _touchMoveDeltaY = p.y - _textContainer->getPositionY();
            _isMovingContainer = true;
        }
        return true;
    };
    listener->onTouchMoved = [this](Touch *touch, Event *event) {
        if (_isMovingContainer) {
            float y = touch->getLocation().y - _touchMoveDeltaY;
            FIX_POS(y, 0, _contentSize.height - _textContainer->getContentSize().height);
            _textContainer->setPositionY(y);
        }
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
        Vec2 p = touch->getStartLocation();
        if (p.x >= _contentSize.width * 2.0/3.0) _curPage++;
        else if (p.x <= _contentSize.width * 1.0/3.0) _curPage--;
        else return;
        if (_curPage < 0) { _curPage = 0; return; }
        else if (_curPage >= _totPages) { _curPage = _totPages - 1; return; }
        this->refreshDisp();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _image = Sprite::create();
    _image->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(_image, 80);
    _textContainer = LayerColor::create(GREY_4B(216, 168),
        _contentSize.width, _contentSize.height * 0.26);
    this->addChild(_textContainer, 85);
    _textLabel = onclr::label("", 12 * onclr::ratio, false, Color3B::BLACK,
        TextHAlignment::LEFT, _contentSize.width);
    _textLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _textLabel->setNormalizedPosition(Vec2::UNIT_Y);
    _textContainer->addChild(_textLabel);
    this->refreshDisp();

    return true;
}

void Tutorial::refreshDisp()
{
    _image->setTexture(String::createWithFormat("tutorial/tutorial_%d.png", _curPage + 1)->getCString());
    _image->setScaleX(_contentSize.width / _image->getContentSize().width);
    _image->setScaleY(_contentSize.height / _image->getContentSize().height);
    _textLabel->setString(_texts[_curPage]);
}

void Tutorial::goBack(Ref *sender)
{
    GO_BACK_ANIMATED;
}
