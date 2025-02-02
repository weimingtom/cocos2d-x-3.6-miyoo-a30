#include "InGameMenuForm.h"
#include "cocos2d/MainScene.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"
#include "ui/UIText.h"
//#include "MenuItemImpl.h"
#include "ui/UIHelper.h"
//#include "tjsGlobalStringMap.h"

using namespace cocos2d;
using namespace cocos2d::ui;

const char * const FileName_NaviBar = "ui/NaviBar.csb";
const char * const FileName_Body = "ui/ListView.csb";

TVPInGameMenuForm * TVPInGameMenuForm::create(const std::string& title, tTJSNI_MenuItem *item) {
	TVPInGameMenuForm *ret = new TVPInGameMenuForm;
	ret->autorelease();
	ret->initFromFile(FileName_NaviBar, FileName_Body, nullptr);
	ret->initMenu(title, item);
	return ret;
}

void TVPInGameMenuForm::bindBodyController(const NodeMap &allNodes) {
	_list = static_cast<ListView*>(allNodes.findController<cocos2d::Node>("list"));
	if (NaviBar.Left) {
		NaviBar.Left->addClickEventListener([this](cocos2d::Ref*){
			TVPMainScene::GetInstance()->popUIForm(this);
		});
	}
}

void TVPInGameMenuForm::bindHeaderController(const NodeMap &allNodes)
{
	_title = static_cast<Button*>(allNodes.findController<cocos2d::Node>("title"));
	if (_title) _title->setEnabled(false);
}

void TVPInGameMenuForm::initMenu(const std::string& title, tTJSNI_MenuItem *item) {
	_list->removeAllItems();
	if (_title) {
		if (title.empty()) {
			_title->setTitleText("xxx");
		} else {
			_title->setTitleText(title);
		}
	}
}

void TVPShowPopMenu(tTJSNI_MenuItem* menu) {
	TVPMainScene::GetInstance()->pushUIForm(TVPInGameMenuForm::create(std::string(), menu));
}
