#define USE_HELLO_3 0
#define USE_HELLO_DESIGN_RES_3 1

#include "MainFileSelectorForm.h"
#include "cocos2d.h"
#include "cocostudio/CocoLoader.h"
#include "cocostudio/CCSSceneReader.h"
//#include "Application.h"
//#include "Platform.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
#include "ui/UIText.h"
#include "ui/UIHelper.h"
#include "ui/UIButton.h"
#include "ui/UIListView.h"
#include "cocos2d/MainScene.h"
//#include "ConfigManager/LocaleConfigManager.h"
//#include "ConfigManager/IndividualConfigManager.h"
#include "GlobalPreferenceForm.h"
#include "IndividualPreferenceForm.h"
#include "MessageBox.h"
//#include "SimpleMediaFilePlayer.h"
//#include "tinyxml2/tinyxml2.h"
//#include "StorageImpl.h"
#include "TipsHelpForm.h"
//#include "XP3RepackForm.h"

using namespace cocos2d;
using namespace cocos2d::ui;

const float UI_ACTION_DUR = 0.3f;
#if USE_HELLO_DESIGN_RES_3
const char * const FileName_NaviBar = "ui/NaviBarWithMenu2.csb";
#else
const char * const FileName_NaviBar = "ui/NaviBarWithMenu.csb";
#endif
const char * const FileName_Body = "ui/TableView.csb";
//const char * const FileName_BottomBar = "ui/BottomBar.csb";
//const char * const FileName_BtnPref = "ui/button/Pref.csb";
const char * const FileName_RecentPathListXML = "recentpath.xml";

bool TVPIsFirstLaunch = false;

std::deque<std::string> _HistoryPath;

static void _AskExit() {

}

bool TVPCheckIsVideoFile(const char *uri);
static std::string _GetHistoryXMLPath() {
	return FileName_RecentPathListXML;
}

static void _LoadHistory() {
	std::string xmlpath = _GetHistoryXMLPath();
	TVPIsFirstLaunch = true;
}

static void _SaveHistory() {
	std::string xmlpath = _GetHistoryXMLPath();

	if (_HistoryPath.empty() && !FileUtils::getInstance()->isFileExist(xmlpath)) return;
}

static void _RemoveHistory(const std::string &path) {
	auto it = std::find(_HistoryPath.begin(), _HistoryPath.end(), path);
	if (it != _HistoryPath.end()) {
		_HistoryPath.erase(it);
		_SaveHistory();
	}
}

static void _AddHistory(const std::string &path) {
	if (!_HistoryPath.empty() && _HistoryPath.front() == path) return;
	_RemoveHistory(path);
	_HistoryPath.push_front(path);
	_SaveHistory();
}

static bool _CheckGameFolder(const std::string &path) {
	bool isValidGameFolder = false;
	std::vector<std::string> subFolders;
#if 1
subFolders.push_back("c:/demosubfile1");
subFolders.push_back("c:/demosubfile2");
subFolders.push_back("c:/demosubfile3");

	while (!isValidGameFolder) {
		if (subFolders.empty()) break;
		isValidGameFolder = _CheckGameFolder(subFolders.back());
		subFolders.pop_back();
	}
#else
subFolders.push_back("demosubfile1");
subFolders.push_back("demosubfile2");
subFolders.push_back("demosubfile3");
isValidGameFolder = true;
#endif
	return isValidGameFolder;
}

TVPMainFileSelectorForm::TVPMainFileSelectorForm()
: _historyList(nullptr), _fileList(nullptr) {
	_menu = nullptr;
}

void TVPMainFileSelectorForm::onEnter()
{
#if defined(_MSC_VER)
	OutputDebugString(L"===============TVPMainFileSelectorForm::onEnter()\n");
#endif
	inherit::onEnter();
	if (_historyList) {
		_historyList->doLayout();
		auto & allcell = _historyList->getItems();
		//for (Widget* cell : allcell) {
		for (auto p_cell = allcell.begin(); p_cell != allcell.end(); ++p_cell)
		{
			auto& cell = *p_cell;
			static_cast<HistoryCell*>(cell)->rearrangeLayout();
		}
	}
}

void TVPMainFileSelectorForm::bindBodyController(const NodeMap &allNodes) {
	TVPBaseFileSelectorForm::bindBodyController(allNodes);

	if (NaviBar.Right) {
		NaviBar.Right->addClickEventListener(std::bind(&TVPMainFileSelectorForm::showMenu, this, std::placeholders::_1));
	}
}

extern "C" void TVPGL_ASM_Test();
void TVPMainFileSelectorForm::show() {
#if defined(_MSC_VER)
	OutputDebugString(L"===============TVPMainFileSelectorForm::show()\n");
#endif
#ifdef _DEBUG
	TVPGL_ASM_Test();
#endif
#if !USE_HELLO_DESIGN_RES_3
	ListHistory(); // filter history data
#else
	ListHistory(); // filter history data
#endif

	bool first = true;
	std::string lastpath;
	if (!_HistoryPath.empty()) lastpath = _HistoryPath.front();
	while (first || (lastpath.size() > RootPathLen && !FileUtils::getInstance()->isDirectoryExist(lastpath))) {
		first = false;
		std::pair<std::string, std::string> split_path = PathSplit(lastpath);
		if (split_path.second.empty()) {
			lastpath.clear();
			break;
		}
		lastpath = split_path.first;
	}
	if (lastpath.size() <= RootPathLen) {

	}
//FIXME:
lastpath = "/home/wmtwmt";
#if !USE_HELLO_DESIGN_RES_3 //see FileName_NaviBar, if skip it, skip here
	ListDir(lastpath); // getCurrentDir()
#else
	ListDir(lastpath); // getCurrentDir()
#endif
	// TODO show usage
}

static const std::string str_startup_tjs("startup.tjs");

bool TVPMainFileSelectorForm::CheckDir(const std::string &path) {
	//for (const FileInfo &info : CurrentDirList) {
	for (auto p_info = CurrentDirList.begin(); p_info != CurrentDirList.end(); ++p_info)
	{
		const auto& info = *p_info; 
		if (info.NameForCompare == str_startup_tjs) return true;
	}
	return false;
}

void TVPMainFileSelectorForm::onCellClicked(int idx) {
	FileInfo info = CurrentDirList[idx];
	TVPBaseFileSelectorForm::onCellClicked(idx);
	int archiveType;
	if (info.IsDir) {
		if (CheckDir(info.FullPath)) {
			startup(info.FullPath);
		}
	}
}

void TVPMainFileSelectorForm::getShortCutDirList(std::vector<std::string> &pathlist) {
	if (!_lastpath.empty()) {
		pathlist.push_back(_lastpath);
	}
	TVPBaseFileSelectorForm::getShortCutDirList(pathlist);
}

TVPMainFileSelectorForm * TVPMainFileSelectorForm::create() {
#if defined(_MSC_VER)
	OutputDebugString(L"=======================>TVPMainFileSelectorForm::create()\n");
#endif
	
	TVPMainFileSelectorForm *ret = new  TVPMainFileSelectorForm();
	ret->autorelease();
	ret->initFromFile();
	ret->show();
	return ret;
}

void TVPMainFileSelectorForm::initFromFile()
{
	_LoadHistory();
//	if (!_HistoryPath.empty())
	{
		CSBReader reader;
		Node *root = reader.Load("ui/MainFileSelector.csb");
		_fileList = reader.findController<cocos2d::Node>("fileList");
		_historyList = static_cast<ListView*>(reader.findController<cocos2d::Node>("recentList"));
		// TODO new node
		_fileOperateMenuNode = _historyList;
		addChild(root);
		Size sceneSize = TVPMainScene::GetInstance()->getUINodeSize();
		setContentSize(sceneSize);
		root->setContentSize(sceneSize);
		ui::Helper::doLayout(root);
	}
#if !USE_HELLO_DESIGN_RES_3 
//skip FileName_NaviBar, that will cause screen white and button green 
	inherit::initFromFile(FileName_NaviBar, FileName_Body, nullptr/*FileName_BottomBar*/, _fileList);
#else
	inherit::initFromFile(FileName_NaviBar, FileName_Body, nullptr/*FileName_BottomBar*/, _fileList);
#endif
}

// std::string _getLastPathFilePath() {
// 	return TVPGetInternalPreferencePath() + "lastpath.txt";
// }

void TVPMainFileSelectorForm::startup(const std::string &path) {
	if (TVPIsFirstLaunch) {
		TVPTipsHelpForm::show()->setOnExitCallback([this, path](){
			scheduleOnce([/*this, path*/=](float) {doStartup(path); }, 0, "startup");
		});
	} else {
		doStartup(path);
	}
}

void TVPMainFileSelectorForm::doStartup(const std::string &path) {
	if (TVPMainScene::GetInstance()->startupFrom(path)) {

	}
}

std::string TVPGetOpenGLInfo();
void TVPOpenPatchLibUrl();

void TVPMainFileSelectorForm::showMenu(Ref*) {
	if (!_menu) {
		Size uiSize = getContentSize();
		CSBReader reader;
		_menu = reader.Load("ui/MenuList.csb");
		_menu->setAnchorPoint(Vec2::ZERO);
		_menu->setPosition(Vec2(uiSize.width, 0));
		_mask = LayerColor::create(Color4B::BLACK, uiSize.width, uiSize.height);
		_mask->setOpacity(0);
		_touchHideMenu = ui::Widget::create();
		_touchHideMenu->setAnchorPoint(Vec2::ZERO);
		_touchHideMenu->setContentSize(uiSize);
		_touchHideMenu->addClickEventListener([this](Ref*) {
			if (isMenuShowed())
				hideMenu(nullptr);
		});
		_mask->addChild(_touchHideMenu);
		addChild(_mask);
		addChild(_menu);
		if (uiSize.width > uiSize.height) {
			uiSize.width /= 3;
		} else {
			uiSize.width *= 0.6f;
		}
		Size menuSize = _menu->getContentSize();
		float scale = uiSize.width / menuSize.width;
		menuSize.height = uiSize.height / scale;
		_menu->setScale(scale);
		_menu->setContentSize(menuSize);
		ui::Helper::doLayout(_menu);

		newLocalPref = reader.findController<cocos2d::Node>("newLocalPref");
		localPref = reader.findController<cocos2d::Node>("localPref");
		sizeNewLocalPref = newLocalPref->getContentSize();
		sizeLocalPref = localPref->getContentSize();

		_menuList = dynamic_cast<ui::ListView*>(reader.findController<cocos2d::Node>("menulist"));
#ifdef _MSC_VER
		//_menuList->setver
#endif

		// button events
		reader.findWidget("btnRotate")->addClickEventListener([](Ref*) {
			TVPMainScene::GetInstance()->pushUIForm(TVPGlobalPreferenceForm::create());
		});
		reader.findWidget("btnGlobalPref")->addClickEventListener([](Ref*) {
			TVPMainScene::GetInstance()->pushUIForm(TVPGlobalPreferenceForm::create());
		});
		reader.findWidget("btnNewLocalPref")->addClickEventListener([this](Ref*) {
			if (1) { //IndividualConfigManager::GetInstance()->CreatePreferenceAt(CurrentPath)) {
				TVPMainScene::GetInstance()->pushUIForm(IndividualPreferenceForm::create());
				hideMenu(nullptr);
			}
		});
		reader.findWidget("btnLocalPref")->addClickEventListener([this](Ref*) {
			onShowPreferenceConfigAt(CurrentPath);
		});
		reader.findWidget("btnHelp")->addClickEventListener([this](Ref*) {
			TVPTipsHelpForm::show();
		});
		bool showSimpleAbout = false;
		if(showSimpleAbout) {
			reader.findWidget("btnAbout")->addClickEventListener([](Ref*) {

			});
			reader.findWidget("btnExit")->addClickEventListener([](Ref*) {

			});
		} else {
			reader.findWidget("btnAbout")->addClickEventListener([](Ref*) {

			});
			reader.findWidget("btnExit")->addClickEventListener([](Ref*) {
#if defined(_MSC_VER)
				OutputDebugString(L"=============>btnExit");
#endif
				_AskExit();
			});
		}
		reader.findWidget("btnRepack")->addClickEventListener([this](Ref*) {
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX)
			TVPProcessXP3Repack(CurrentPath);
#endif
			hideMenu(nullptr);
		});
		reader.findWidget("btnNewFolder")->addClickEventListener([this](Ref*) {
			hideMenu(nullptr);
		});

	}
	const Size &uiSize = getContentSize();
	const Vec2 &pos = _menu->getPosition();
	const Size &size = _menu->getContentSize();
	float w = size.width * _menu->getScale();
	if (pos.x > uiSize.width - w / 10.0f) {
		if (0) { //IndividualConfigManager::CheckExistAt(CurrentPath)) {
			localPref->setVisible(true);
			localPref->setContentSize(sizeLocalPref);
			newLocalPref->setVisible(false);
			newLocalPref->setContentSize(Size::ZERO);
		} else {
			newLocalPref->setVisible(true);
			newLocalPref->setContentSize(sizeNewLocalPref);
			localPref->setVisible(false);
			localPref->setContentSize(Size::ZERO);
		}
		_menuList->requestDoLayout();
		_mask->stopAllActions();
		_mask->runAction(FadeTo::create(UI_ACTION_DUR, 128));
		_menu->stopAllActions();
		_menu->runAction(EaseQuadraticActionOut::create(
			MoveTo::create(UI_ACTION_DUR, Vec2(uiSize.width - w, pos.y))));
		_touchHideMenu->setTouchEnabled(true);
	}
}

void TVPMainFileSelectorForm::hideMenu(cocos2d::Ref*)
{
	if (!_menu) return;
	_mask->stopAllActions();
	_mask->runAction(FadeOut::create(UI_ACTION_DUR));
	_menu->stopAllActions();
	_menu->runAction(EaseQuadraticActionOut::create(
		MoveTo::create(UI_ACTION_DUR, Vec2(getContentSize().width, _menu->getPositionY()))));
	_touchHideMenu->setTouchEnabled(false);
}

bool TVPMainFileSelectorForm::isMenuShowed()
{
	if (!_menu) return false;
	const Vec2 &pos = _menu->getPosition();
	const Size &size = _menu->getContentSize();
	float w = size.width * _menu->getScale();
	if (pos.x < getContentSize().width - w * 0.9f) {
		return true;
	}
	return false;
}

bool TVPMainFileSelectorForm::isMenuShrinked()
{
	if (!_menu) return true;
	const Vec2 &pos = _menu->getPosition();
	const Size &size = _menu->getContentSize();
	float w = size.width * _menu->getScale();
	if (pos.x > getContentSize().width - w / 10.0f) {
		return false;
	}
	return true;
}

void TVPMainFileSelectorForm::onShowPreferenceConfigAt(const std::string &path)
{
	if (1) { //if (IndividualConfigManager::GetInstance()->UsePreferenceAt(path)) {
		TVPMainScene::GetInstance()->pushUIForm(IndividualPreferenceForm::create());
	}
}

void TVPMainFileSelectorForm::ListHistory()
{
	if (!_historyList) return;
	_historyList->removeAllChildren();
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX)
	HistoryCell *nullcell = new HistoryCell();
	nullcell->init();
#else
#if 0
	__debugbreak(); throw;
#else
	HistoryCell *nullcell = HistoryCell::create("", "", "", "");
#endif
#endif
	Size cellsize = _historyList->getContentSize();
	cellsize.height = 100;
	nullcell->setContentSize(cellsize);
	_historyList->pushBackCustomItem(nullcell);
	for (auto it = _HistoryPath.begin(); it != _HistoryPath.end();) {
		const std::string &fullpath = *it;
		HistoryCell *cell;
		if (1) { //TVPCheckExistentLocalFile(fullpath) || TVPCheckExistentLocalFolder(fullpath)) {
			std::pair<std::string, std::string> split_path = PathSplit(fullpath);
			std::string lastname = split_path.second;
			std::string path = split_path.first;
			split_path = PathSplit(path);
			cell = HistoryCell::create(fullpath, split_path.first + "/", split_path.second, "/" + lastname);
			Widget::ccWidgetClickCallback funcConf;
			//if (TVPCheckExistentLocalFile(path + "/Kirikiroid2Preference.xml"))
			//	funcConf = [this, path](Ref*){ onShowPreferenceConfigAt(path); };
			cell->initFunction(std::bind(&TVPMainFileSelectorForm::RemoveHistoryCell, this, std::placeholders::_1, cell),
				[this, path](Ref*){ ListDir(path); }, funcConf, [this, fullpath](Ref*) { startup(fullpath); });
			Size cellsize = cell->getContentSize();
			cellsize.width = _historyList->getContentSize().width;
			cell->setContentSize(cellsize);
			_historyList->pushBackCustomItem(cell);
			++it;
		} else {
			it = _HistoryPath.erase(it);
			continue;
		}
	}
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX)
	nullcell = new HistoryCell();
	nullcell->init();
#else
#if 0
	__debugbreak(); throw;
#else
	nullcell = HistoryCell::create("", "", "", "");
#endif
#endif
	nullcell->setContentSize(cellsize);
	_historyList->pushBackCustomItem(nullcell);
}

void TVPMainFileSelectorForm::RemoveHistoryCell(cocos2d::Ref* btn, HistoryCell* cell)
{
	static_cast<Widget*>(btn)->setEnabled(false);
	cell->runAction(Sequence::createWithTwoActions(
		EaseQuadraticActionOut::create(MoveBy::create(0.25, Vec2(-cell->getContentSize().width, 0))),
		CallFuncN::create([this](Node* p){
		HistoryCell* cell = static_cast<HistoryCell*>(p);
		ssize_t idx = _historyList->getIndex(cell);
		if (idx < 0) return;
		_historyList->removeItem(idx);
	})));
	_RemoveHistory(cell->getFullpath());
	_SaveHistory();
}

void TVPMainFileSelectorForm::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
		if (isMenuShowed()) {
			hideMenu(nullptr);
		} else {
			_AskExit();
		}
	} else if (keyCode == EventKeyboard::KeyCode::KEY_MENU) {
		if (isMenuShrinked()) {
			showMenu(nullptr);
		}
	} else {
		inherit::onKeyPressed(keyCode, event);
	}
}

void TVPMainFileSelectorForm::HistoryCell::initInfo(const std::string &fullpath, const std::string &prefix, const std::string &pathname, const std::string &filename)
{
	_fullpath = fullpath;

	CSBReader reader;
	_root = reader.Load("ui/RecentListItem.csb");
	_scrollview = static_cast<cocos2d::ui::ScrollView*>(reader.findController<cocos2d::Node>("scrollview"));
	_btn_delete = static_cast<cocos2d::ui::Widget*>(reader.findController<cocos2d::Node>("btn_delete"));
	_btn_jump = static_cast<cocos2d::ui::Widget*>(reader.findController<cocos2d::Node>("btn_jump"));
	_btn_conf = static_cast<cocos2d::ui::Widget*>(reader.findController<cocos2d::Node>("btn_conf"));
	_btn_play = static_cast<cocos2d::ui::Widget*>(reader.findController<cocos2d::Node>("btn_play"));
	_prefix = static_cast<cocos2d::ui::Text*>(reader.findController<cocos2d::Node>("prefix"));
	_path = static_cast<cocos2d::ui::Text*>(reader.findController<cocos2d::Node>("path"));
	_file = static_cast<cocos2d::ui::Text*>(reader.findController<cocos2d::Node>("file"));
	_panel_delete = reader.findController<cocos2d::Node>("panel_delete");
	if (!_panel_delete) _panel_delete = _btn_delete;
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX)
//FIXME:
	_scrollview->setScrollBarEnabled(false);
#else
	//_scrollview->setInertiaScrollEnabled(false);
#endif
	_scrollview->setPropagateTouchEvents(true);

	_prefix->setString(prefix);
	_path->setString(pathname);
	_file->setString(filename);

	setContentSize(_root->getContentSize());
	addChild(_root);
}

void TVPMainFileSelectorForm::HistoryCell::rearrangeLayout()
{
	if (!_root) return;
	_root->setContentSize(this->getContentSize());
	ui::Helper::doLayout(_root);
	Vec2 pt = Vec2::ZERO;
	pt.x = _file->getContentSize().width;
	Vec2 ptWorld = _file->convertToWorldSpace(pt);
	Size viewSize = _scrollview->getContentSize();
	Node *container = _scrollview->getInnerContainer();
	pt = container->convertToNodeSpace(ptWorld);
	float btnw = _panel_delete->getContentSize().width;
	float offsetx = 0;
	if (pt.x > viewSize.width) {
		float neww = pt.x;
		pt.y = 0; pt.x = _path->getContentSize().width;
		ptWorld = _path->convertToWorldSpace(pt);
		pt = container->convertToNodeSpace(ptWorld);
		if (pt.x > viewSize.width) {
			offsetx = viewSize.width - pt.x;
		}
		viewSize.width = neww;
	}
	_panel_delete->setPositionX(viewSize.width + btnw);
	viewSize.width += btnw + btnw;
	_scrollview->setInnerContainerSize(viewSize);
	container->setPosition(offsetx, 0);
}

void TVPMainFileSelectorForm::HistoryCell::initFunction(const ccWidgetClickCallback &funcDel, const ccWidgetClickCallback &funcJump, const ccWidgetClickCallback &funcConf, const ccWidgetClickCallback &funcPlay)
{
	_btn_delete->addClickEventListener(funcDel);
	_btn_play->addClickEventListener(funcPlay);
	if (funcConf) _btn_conf->addClickEventListener(funcConf);
	else _btn_conf->setVisible(false);
	_btn_jump->addClickEventListener(funcJump);
}

void TVPMainFileSelectorForm::HistoryCell::onSizeChanged()
{

}
