#include <vector>
#include "GlobalPreferenceForm.h"
//#include "ConfigManager/LocaleConfigManager.h"
#include "ui/UIButton.h"
#include "cocos2d/MainScene.h"
#include "ui/UIListView.h"
//#include "ConfigManager/GlobalConfigManager.h"
#include "platform/CCFileUtils.h"
//#include "Platform.h"

using namespace cocos2d;
using namespace cocos2d::ui;
#define GLOBAL_PREFERENCE

const char * const FileName_NaviBar = "ui/NaviBar.csb";
const char * const FileName_Body = "ui/ListView.csb";
#include "PreferenceConfig.h"

TVPGlobalPreferenceForm * TVPGlobalPreferenceForm::create(const tPreferenceScreen *config) {
	Initialize();
	if (!config) config = &RootPreference;
	TVPGlobalPreferenceForm *ret = new TVPGlobalPreferenceForm();
	ret->autorelease();
	ret->initFromFile(FileName_NaviBar, FileName_Body, nullptr);
	PrefListSize = ret->PrefList->getContentSize();
	ret->initPref(config);
	return ret;
}

static void WalkConfig(tPreferenceScreen* pref) {
	//for (iTVPPreferenceInfo* info : pref->Preferences) {
	auto& list = pref->Preferences;
	for (auto p_info = list.begin(); p_info != list.end(); ++p_info)
	{
		const auto& info = *p_info;
		info->InitDefaultConfig();
		tPreferenceScreen* subpref = info->GetSubScreenInfo();
		if (subpref) {
			WalkConfig(subpref);
		}
	}
}

void TVPGlobalPreferenceForm::Initialize()
{
	static bool Inited = false;
	if (!Inited) {
		Inited = true;

		initAllConfig();
		WalkConfig(&RootPreference);
		WalkConfig(&SoftRendererOptPreference);
		WalkConfig(&OpenglOptPreference);
	}
}
