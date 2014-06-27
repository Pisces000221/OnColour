#include "Global.h"
#include "cocos2d.h"
using namespace cocos2d;

namespace onclr {

cocos2d::Size vsize;
void init()
{
    vsize = Director::getInstance()->getVisibleSize();
}

#define GLOBAL_REGULAR_FONT "fonts/FiraSans_400.otf"
#define GLOBAL_BOLD_FONT    "fonts/FiraSans_700.otf"

Label *label(const char *str, int fontsize, bool isbold,
    Color3B colour, TextHAlignment alignment, int linewidth)
{
    Label *r = Label::createWithTTF(
        TTFConfig(isbold ? GLOBAL_BOLD_FONT : GLOBAL_REGULAR_FONT, fontsize),
        str, alignment, linewidth);
    r->setColor(colour);
    return r;
}

}
