#include "Global.h"
#include "cocos2d.h"
using namespace cocos2d;

namespace onclr {

// Constants first
const Size mapsize = Size(1200, 800);
const float photongen_mintime = 3.0f;
const float photongen_maxtime = 6.0f;
const float player_radius = 30.0f;
const float player_colour_lost = 8.0f;
const float player_colour_warning = 64.0f;
const float photon_minradius = 36.0f;
const float photon_maxradius = 72.0f;
const int photon_mincolourval = 64;
const int photon_maxcolourval = 255;
const int photon_minvelocity = 30;
const int photon_maxvelocity = 120;
const float photon_minhugtime = 3.0f;
const float photon_maxhugtime = 5.0f;

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
