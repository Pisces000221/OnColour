#include "Global.h"
#include "cocos2d.h"
using namespace cocos2d;

namespace onclr {

// Constants first
const Size mapsize = Size(1200, 800);
const float photongen_mintime = 1.8f;
const float photongen_maxtime = 4.0f;
const float player_radius = 30.0f;
const float player_colour_initial = 156.0f;
const float player_colour_lost = 8.0f;
const float player_colour_warning = 64.0f;
const float player_colour_fever = 128.0f;
const float photon_minradius = 36.0f;
const float photon_maxradius = 72.0f;
const int photon_mincolourval = 64;
const int photon_maxcolourval = 255;
const int photon_minvelocity = 30;
const int photon_maxvelocity = 120;
const float max_difficulty_rate = 2.5f;
const float difficulty_rate_increment = 0.02f;
const float photon_minhugtime = 3.0f;
const float photon_maxhugtime = 5.0f;
const float sinevel_photon_minperiod = 0.9f;
const float sinevel_photon_maxperiod = 2.7f;
const float sinevel_photon_minam = 0.3f;
const float sinevel_photon_maxam = 0.7f;
const float normal_photon_possib = 0.6f;
const float sinevel_photon_possib = 0.35f;
const float bomb_possib = 0.05f;

cocos2d::Size vsize;
float ratio;
float bubble_scale;
cocos2d::LanguageType language;
std::map<std::string, std::string> locale;
void init()
{
    language = (cocos2d::LanguageType)
        (UserDefault::getInstance()->getIntegerForKey("Language",
        (int)(Application::getInstance()->getCurrentLanguage())));
    std::string langNames[] = { "en-UK", "zh-CN" };
    int langCount = 2;
    // Unsupported language?
    if ((int)language >= langCount) language = LanguageType::ENGLISH;
    // Read localization info
    FILE *fp = fopen(onclr::readableAssetFile("locale/" + langNames[(int)language] + ".locale").c_str(), "r");
    std::string k, v;
    while (!feof(fp)) {
        char _k[1024], _v[1024];
        fgets(_k, 1024, fp);
        fgets(_v, 1024, fp);
        //http://blog.csdn.net/butterfly_dreaming/article/details/10142443
        k = _k; k.erase(k.find_last_not_of("\n") + 1);
        v = _v; v.erase(v.find_last_not_of("\n") + 1);
        locale[k] = v;
    }
    fclose(fp);
    vsize = Director::getInstance()->getVisibleSize();
    ratio = vsize.width / 480.0;
    bubble_scale = (ratio - 1) * 0.4 + 1;
}

#define GLOBAL_REGULAR_FONT locale["__FONTNAME_R__"].c_str()
#define GLOBAL_BOLD_FONT    locale["__FONTNAME_B__"].c_str()

Label *label(std::string str, int fontsize, bool isbold,
    Color3B colour, TextHAlignment alignment, int linewidth)
{
    auto find_result = locale.find(str);
    if (find_result != locale.end()) str = find_result->second;
    Label *r = Label::createWithTTF(
        TTFConfig(isbold ? GLOBAL_BOLD_FONT : GLOBAL_REGULAR_FONT, fontsize),
        str, alignment, linewidth);
    r->setColor(colour);
    return r;
}

std::string readableAssetFile(std::string filename)
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    if (path[path.length() - 1] != '/') path += '/';
    std::string textFileName =
        FileUtils::getInstance()->getWritablePath() + "mydata.dat";
    Data data = FileUtils::getInstance()->getDataFromFile(filename);
    FILE *fp = fopen(textFileName.c_str(), "w");
    fwrite(data.getBytes(), 1, data.getSize(), fp);
    fclose(fp);
    return textFileName;
}

}
