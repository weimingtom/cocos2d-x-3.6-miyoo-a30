#define USE_HELLO_2 0
#define USE_HELLO_DESIGN_RES_2 1

#include "MainScene.h"
#include "cocos2d.h"
#include "cocos-ext.h"
//#include "tjsCommHead.h"
//#include "StorageIntf.h"
//#include "EventIntf.h"
//#include "SysInitImpl.h"
//#include "WindowImpl.h"
//#include "LayerBitmapIntf.h"
//#include "LayerBitmapImpl.h"
#include "ui/BaseForm.h"
#include "ui/GameMainMenu.h"
#include "ui/UIHelper.h"
//#include "TickCount.h"
//#include "Random.h"
//#include "UtilStreams.h"
//#include "vkdefine.h"
#include "base/CCEventListenerController.h"
#include "base/CCController.h"
//#include "ConfigManager/IndividualConfigManager.h"
//#include "Platform.h"
#include "ui/ConsoleWindow.h"
#include "ui/FileSelectorForm.h"
#include "ui/DebugViewLayerForm.h"
//#include "Application.h"
//#include "ScriptMgnIntf.h"
//#include "win32/TVPWindow.h"
//#include "VelocityTracker.h"
//#include "SystemImpl.h"
//#include "RenderManager.h"
//#include "VideoOvlIntf.h"
//#include "Exception.h"
//#include "win32/SystemControl.h"
#ifndef _MSC_VER
#include <unistd.h> //usleep
#endif

USING_NS_CC;

enum SCENE_ORDER {
	GAME_SCENE_ORDER,
	GAME_CONSOLE_ORDER,
	GAME_WINMGR_ORDER, // also for the virtual mouse cursor
	GAME_MENU_ORDER,
	UI_NODE_ORDER,
};

const float UI_CHANGE_DURATION = 0.3f;
//class TVPWindowLayer;
//static TVPWindowLayer *_lastWindowLayer, *_currentWindowLayer;
class TVPWindowManagerOverlay;
static TVPWindowManagerOverlay *_windowMgrOverlay = nullptr;
static TVPConsoleWindow* _consoleWin = nullptr;
static float _touchMoveThresholdSq;
static cocos2d::Node *_mouseCursor;
static float _mouseCursorScale;
static Vec2 _mouseTouchPoint, _mouseBeginPoint;
static std::set<Touch*> _mouseTouches;
//static tTVPMouseButton _mouseBtn;
static int _touchBeginTick;
static bool _virutalMouseMode = false;
static bool _mouseMoved, _mouseClickedDown;
//static tjs_uint8 _scancode[0x200];
static Label *_fpsLabel = nullptr;

//#include "CCKeyCodeConv.h"

#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#endif

static void(*_postUpdate)() = nullptr;
void TVPSetPostUpdateEvent(void(*f)()) { _postUpdate = f; }

static void _refadeMouseCursor() {
	_mouseCursor->stopAllActions();
	_mouseCursor->setOpacity(255);
	_mouseCursor->runAction(Sequence::createWithTwoActions(DelayTime::create(3), FadeOut::create(0.3)));
}

struct tTVPCursor {
	Node *RootNode;
	Action *Anim/* = nullptr*/;
	tTVPCursor() : Anim(nullptr) {}
};

#pragma pack(push)
#pragma pack(1)
enum eIconType {
	eIconTypeNone,
	eIconTypeICO,
	eIconTypeCUR
};

struct ICONDIR {
	uint16_t idReserved; // must be 0
	uint16_t idType; //eIconType
	uint16_t idCount;
};

struct ICODIREntry {
	uint8_t bWidth; // 0 -> 256
	uint8_t bHeight; // 0 -> 256
	uint8_t bColorCount;
	uint8_t bReserved;
	union {
		struct {
			uint16_t wPlanes;
			uint16_t wBitCount;
		};
		struct {
			uint16_t wHotSpotX;
			uint16_t wHotSpotY;
		};
	};
	uint32_t dwBytesInRes;
	uint32_t dwImageOffset;
};
#pragma pack(pop)

class TVPWindowManagerOverlay : public iTVPBaseForm {
public:
	static TVPWindowManagerOverlay *create() {
		TVPWindowManagerOverlay* ret = new TVPWindowManagerOverlay();
		ret->autorelease();
		ret->initFromFile(nullptr, "ui/WinMgrOverlay.csb", nullptr);
		return ret;
	}
	virtual void rearrangeLayout() {
		Size sceneSize = TVPMainScene::GetInstance()->getGameNodeSize();
		setContentSize(sceneSize);
		RootNode->setContentSize(sceneSize);
		ui::Helper::doLayout(RootNode);
	}

	virtual void bindBodyController(const NodeMap &allNodes) {
		_left = static_cast<ui::Button*>(allNodes.findController<cocos2d::Node>("left"));
		_right = static_cast<ui::Button*>(allNodes.findController<cocos2d::Node>("right"));
		_ok = static_cast<ui::Button*>(allNodes.findController<cocos2d::Node>("ok"));

		auto funcUpdate = std::bind(&TVPWindowManagerOverlay::updateButtons, this);

		_left->addClickEventListener([=](Ref*){
			_left->setVisible(false);
			_right->setVisible(false);
		});

		_right->addClickEventListener([=](Ref*){
			_left->setVisible(false);
			_right->setVisible(false);
		});

		_ok->addClickEventListener([](Ref*){
			TVPMainScene::GetInstance()->showWindowManagerOverlay(false);
		});

		ui::Button* fillscr = static_cast<ui::Button*>(allNodes.findController<cocos2d::Node>("fillscr"));
		fillscr->addClickEventListener([](Ref*){
			//if (!_currentWindowLayer) return;
		});

		updateButtons();
	}

	void updateButtons() {
		//if (!_currentWindowLayer) return;
		if (_left) {

		}
		if (_right) {

		}
	}
	
private:
	ui::Button *_left, *_right, *_ok;
};

static std::function<bool(Touch *, Event *)> _func_mask_layer_touchbegan;

class MaskLayer : public LayerColor {
public:
	static LayerColor * create(const Color4B& color, GLfloat width, GLfloat height) {
		LayerColor * layer = LayerColor::create(color, width, height);
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		if (_func_mask_layer_touchbegan) {
			listener->onTouchBegan = _func_mask_layer_touchbegan;
			_func_mask_layer_touchbegan = nullptr;
		} else {
			listener->onTouchBegan = [](Touch *, Event *)->bool{return true; };
		}
		listener->onTouchMoved = [](Touch *, Event *) {};
		listener->onTouchEnded = [](Touch *, Event *) {};
		listener->onTouchCancelled = [](Touch *, Event *) {};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);

		return layer;
	}
};

static TVPMainScene *_instance = nullptr;

TVPMainScene* TVPMainScene::GetInstance() {
	return _instance;
}

TVPMainScene* TVPMainScene::CreateInstance() {
	_instance = create();
	return _instance;
}

void TVPMainScene::initialize() {
	auto glview = cocos2d::Director::getInstance()->getOpenGLView();
	Size screenSize = glview->getFrameSize();
	Size designSize = glview->getDesignResolutionSize();
	ScreenRatio = screenSize.height / designSize.height;
#if !USE_HELLO_DESIGN_RES_2
	designSize.width = designSize.height * screenSize.width / screenSize.height;
#endif
	initWithSize(designSize);
	addChild(LayerColor::create(Color4B::BLACK, designSize.width, designSize.height));
	GameNode = cocos2d::Node::create();
	// horizontal
	//std::swap(designSize.width, designSize.height);
	GameNode->setContentSize(designSize);
	UINode = cocos2d::Node::create();
	UINode->setContentSize(designSize);
	UINode->setAnchorPoint(Vec2::ZERO);
	UINode->setPosition(Vec2::ZERO);
	UISize = designSize;
	addChild(UINode, UI_NODE_ORDER);
	GameNode->setAnchorPoint(Vec2(0, 0));
	//GameNode->setRotation(-90);
	//GameNode->setPosition(getContentSize() / 2);
	addChild(GameNode, GAME_SCENE_ORDER);

	EventListenerKeyboard* keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(TVPMainScene::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(TVPMainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithFixedPriority(keylistener, 1);

	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(TVPMainScene::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(TVPMainScene::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(TVPMainScene::onTouchEnded, this);
	_touchListener->onTouchCancelled = CC_CALLBACK_2(TVPMainScene::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
#ifndef _MSC_VER
	EventListenerController *ctrllistener = EventListenerController::create();
	ctrllistener->onAxisEvent = CC_CALLBACK_3(TVPMainScene::onAxisEvent, this);
	ctrllistener->onKeyDown = CC_CALLBACK_3(TVPMainScene::onPadKeyDown, this);
	ctrllistener->onKeyUp = CC_CALLBACK_3(TVPMainScene::onPadKeyUp, this);
	ctrllistener->onKeyRepeat = CC_CALLBACK_3(TVPMainScene::onPadKeyRepeat, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ctrllistener, this);
#else
	//__debugbreak(); throw; //windows does not support EventListenerController
	CCLOG("=====================windows does not support EventListenerController");
#endif
}

TVPMainScene * TVPMainScene::create() {
	TVPMainScene * ret = new TVPMainScene;
	ret->initialize();
	ret->autorelease();

	_touchMoveThresholdSq = Device::getDPI() / 10;
	_touchMoveThresholdSq *= _touchMoveThresholdSq;
	return ret;
}

void TVPMainScene::pushUIForm(cocos2d::Node *ui, eEnterAni ani) {
	int n = UINode->getChildrenCount();
	if (ani == eEnterAniNone) {
		UINode->addChild(ui);
	} else if (ani == eEnterAniOverFromRight) {
		if (n > 0) {
			Size size = UINode->getContentSize();
			cocos2d::Node *lastui = UINode->getChildren().back();
			lastui->runAction(EaseQuadraticActionOut::create(MoveTo::create(UI_CHANGE_DURATION,
				Vec2(size.width / -5, 0))));
			cocos2d::Node *ColorMask = MaskLayer::create(Color4B(0, 0, 0, 0), size.width, size.height);
			ColorMask->setPosition(Vec2(-size.width, 0));
			ui->addChild(ColorMask);
			ColorMask->runAction(FadeTo::create(UI_CHANGE_DURATION, 128));
			ui->setPosition(size.width, 0);
			ui->runAction(EaseQuadraticActionOut::create(MoveTo::create(UI_CHANGE_DURATION, Vec2::ZERO)));
			runAction(Sequence::createWithTwoActions(DelayTime::create(UI_CHANGE_DURATION), CallFunc::create([=](){
				ColorMask->removeFromParent();
			})));
		}
		UINode->addChild(ui);
	} else if (ani == eEnterFromBottom) {
		Size size = UINode->getContentSize();
		cocos2d::Node *ColorMask = MaskLayer::create(Color4B(0, 0, 0, 0), size.width, size.height);
		ColorMask->runAction(FadeTo::create(UI_CHANGE_DURATION, 128));
		ui->setPositionY(-ui->getContentSize().height);
		ColorMask->addChild(ui);
		UINode->addChild(ColorMask);
		ui->runAction(EaseQuadraticActionOut::create(MoveTo::create(UI_CHANGE_DURATION, Vec2::ZERO)));
	}
}

void TVPMainScene::popUIForm(cocos2d::Node *form, eLeaveAni ani) {
	int n = UINode->getChildrenCount();
	if (n <= 0) return;
	if (n == 1) {

	}
	auto children = UINode->getChildren();
	if (ani == eLeaveAniNone) {
		if (n > 1) {
			Node *lastui = children.at(n - 2);
			lastui->setPosition(0, 0);
		}
		Node *ui = children.back();
		if (form) CCAssert(form == ui, "must be the same form");
		ui->removeFromParent();
	} else if (ani == eLeaveAniLeaveFromLeft) {
		Node *ui = children.back();
		if (form) CCAssert(form == ui, "must be the same form");
		Size size = UINode->getContentSize();
		if (n > 1) {
			Node *lastui = children.at(n - 2);
			lastui->setPosition(size.width / -5, 0);
			lastui->runAction(EaseQuadraticActionOut::create(MoveTo::create(UI_CHANGE_DURATION, Vec2::ZERO)));
		}
		cocos2d::Node *ColorMask = MaskLayer::create(Color4B(0, 0, 0, 128), size.width, size.height);
		ColorMask->setPosition(Vec2(-size.width, 0));
		ui->addChild(ColorMask);
		ColorMask->runAction(FadeOut::create(UI_CHANGE_DURATION));
		ui->runAction(EaseQuadraticActionOut::create(MoveTo::create(UI_CHANGE_DURATION, Vec2(size.width, 0))));
		runAction(Sequence::createWithTwoActions(DelayTime::create(UI_CHANGE_DURATION), CallFunc::create([=](){
			ui->removeFromParent();
		})));
	} else if (ani == eLeaveToBottom) {
		cocos2d::Node *ColorMask = children.back();
		ColorMask->runAction(FadeOut::create(UI_CHANGE_DURATION));
		Node *ui = ColorMask->getChildren().at(0);
		if (form) CCAssert(form == ui, "must be the same form");
		ui->runAction(EaseQuadraticActionIn::create(MoveTo::create(UI_CHANGE_DURATION, Vec2(0, -ui->getContentSize().height))));
		runAction(Sequence::createWithTwoActions(DelayTime::create(UI_CHANGE_DURATION), CallFunc::create([=](){
			ColorMask->removeFromParent();
		})));
	}
}

bool TVPMainScene::startupFrom(const std::string &path) {
	// startup from dir
#ifdef _MSC_VER
	//TVPSetSystemOption("outputlog", "yes");
// 	TVPSetSystemOption("ogl_dup_target", "yes");
	//_set_new_handler(_no_memory_cb_vc);
#endif
	if (UINode->getChildrenCount()) {
		popUIForm(nullptr);
	}

	// 	if (pGlobalCfgMgr->GetValueBool("rot_screen_180", false)) {
// 		GameNode->setRotation(90);
// 	}

	scheduleOnce(std::bind(&TVPMainScene::doStartup, this, std::placeholders::_1, path), 0, "startup");

	return true;
}

void TVPMainScene::doStartup(float dt, std::string path) {
	unschedule("startup");
	_consoleWin = TVPConsoleWindow::create(14, nullptr);

	auto glview = cocos2d::Director::getInstance()->getOpenGLView();
	Size screenSize = glview->getFrameSize();
	float scale = screenSize.height / getContentSize().height;
	_consoleWin->setScale(1 / scale);
	_consoleWin->setContentSize(getContentSize() * scale);
	_consoleWin->setFontSize(16);
	GameNode->addChild(_consoleWin, GAME_CONSOLE_ORDER);
	// update one frame
	update(0);
	//_ResotreGLStatues(); // already in update()
	_gameMenu = TVPGameMainMenu::create(1.0f);
	GameNode->addChild(_gameMenu, GAME_MENU_ORDER);
	_gameMenu->shrinkWithTime(1);
	if (_consoleWin) {
		_consoleWin->removeFromParent();
		_consoleWin = nullptr;
		scheduleUpdate();

		cocos2d::Director::getInstance()->purgeCachedData();
	}
}

void TVPMainScene::update(float delta) {
//	if (_currentWindowLayer) _currentWindowLayer->UpdateOverlay();
	//_ResotreGLStatues();
	if (_postUpdate) _postUpdate();
	if (_fpsLabel) {
		unsigned int drawCount;
		uint64_t vmemsize;
		static timeval _lastUpdate;
		//static int _lastUpdateReq = gettimeofday(&_lastUpdate, nullptr);
		struct timeval now;
		gettimeofday(&now, nullptr);
		float _deltaTime = (now.tv_sec - _lastUpdate.tv_sec) + (now.tv_usec - _lastUpdate.tv_usec) / 1000000.0f;
		_lastUpdate = now;

		static float prevDeltaTime = 0.016f; // 60FPS
		static const float FPS_FILTER = 0.10f;
		static float _accumDt = 0;
		static unsigned int prevDrawCount = 0;
		_accumDt += _deltaTime;
		float dt = _deltaTime * FPS_FILTER + (1 - FPS_FILTER) * prevDeltaTime;
		prevDeltaTime = dt;
		if (drawCount > prevDrawCount)
			prevDrawCount = drawCount;

		char buffer[30];
		if (_accumDt > 0.1f) {
			sprintf(buffer, "%.1f (%d draws)", 1 / dt, drawCount);
			_fpsLabel->setString(buffer);
//#ifdef _MSC_VER
			std::string msg = buffer; msg += "\n";
			//sprintf(buffer, "%.2f MB", TVPGetGraphicCacheTotalBytes() / (float)(1024 * 1024));
			vmemsize >>= 10;
			sprintf(buffer, "%d MB(%.2f MB) %d MB\n", 0, (float)vmemsize / 1024.f, 0);
			msg += buffer;
			_fpsLabel->setString(msg);
//#endif
			_accumDt = 0;
			prevDrawCount = 0;
		}
	}
}

cocos2d::Size TVPMainScene::getUINodeSize() {
	return UINode->getContentSize();
}

void TVPMainScene::addLayer(TVPWindowLayer* lay) {

}

void TVPMainScene::rotateUI() {
	float rot = UINode->getRotation();
	if (rot < 1) {
		UINode->setRotation(90);
		UINode->setContentSize(Size(UISize.height, UISize.width));
	} else {
		UINode->setRotation(0);
		UINode->setContentSize(UISize);
	}
	//for (Node* ui : UINode->getChildren()) {
	auto& children = UINode->getChildren();
	for (auto p_ui = children.begin(); p_ui != children.end(); ++p_ui)
	{
		auto& ui = *p_ui;
		static_cast<iTVPBaseForm*>(ui)->rearrangeLayout();
	}
}

void TVPMainScene::setMaskLayTouchBegain(const std::function<bool(cocos2d::Touch *, cocos2d::Event *)> &func) {
	_func_mask_layer_touchbegan = func;
}

static float _getUIScale() {
	auto glview = Director::getInstance()->getOpenGLView();
	float factor = (glview->getScaleX() + glview->getScaleY()) / 2;
	factor /= Device::getDPI(); // inch per pixel
	Size screenSize = glview->getFrameSize();
	Size designSize = glview->getDesignResolutionSize();
	designSize.width = designSize.height * screenSize.width / screenSize.height;
	screenSize.width = factor * designSize.width;
#ifdef _WIN32
	//if (screenSize.width > 3.5433) return 0.35f; // 7 inch @ 16:9 device
	return 0.35f;
#endif
// 	char tmp[128];
// 	sprintf(tmp, "screenSize.width = %f", (float)screenSize.width);
// 	TVPPrintLog(tmp);
// #if CC_PLATFORM_IOS == CC_TARGET_PLATFORM
// 	return /*sqrtf*/(0.0005f / factor) * screenSize.width;
// #else
	return /*sqrtf*/(0.0005f / factor) * screenSize.width;
//#endif
}

float TVPMainScene::getUIScale() {
	static float uiscale = _getUIScale();
	return uiscale;
}

void TVPMainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	Vector<Node*>& uiChild = UINode->getChildren();
	if (!uiChild.empty()) {
		iTVPBaseForm* uiform = dynamic_cast<iTVPBaseForm*>(uiChild.back());
		if (uiform) uiform->onKeyPressed(keyCode, event);
		return;
	}
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_MENU:
		if (UINode->getChildren().empty()) {
			if (_gameMenu) _gameMenu->toggle();
		}
		return;
		break;
	case EventKeyboard::KeyCode::KEY_BACK:
		if (!UINode->getChildren().empty()) {
			return;
		}
		if (_gameMenu && !_gameMenu->isShrinked()) {
			_gameMenu->shrink();
			return;
		}
		keyCode = EventKeyboard::KeyCode::KEY_ESCAPE;
		break;
#ifdef _DEBUG
	case EventKeyboard::KeyCode::KEY_PAUSE:
		GameNode->addChild(DebugViewLayerForm::create());
		return;
	case EventKeyboard::KeyCode::KEY_F12:
		break;
#endif
	default:
		break;
	}
}

void TVPMainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
#ifdef _DEBUG
	if (keyCode == EventKeyboard::KeyCode::KEY_PAUSE) return;
#endif
	if (keyCode == EventKeyboard::KeyCode::KEY_MENU) return;
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) keyCode = EventKeyboard::KeyCode::KEY_ESCAPE;
	if (keyCode == EventKeyboard::KeyCode::KEY_PLAY) { // auto play

	}
}

TVPMainScene::TVPMainScene() {
	_gameMenu = nullptr;
	_windowMgrOverlay = nullptr;
}

void TVPMainScene::showWindowManagerOverlay(bool bVisible) {
	if (bVisible) {
		if (!_windowMgrOverlay) {
			_windowMgrOverlay = TVPWindowManagerOverlay::create();
			GameNode->addChild(_windowMgrOverlay, GAME_WINMGR_ORDER);
			_gameMenu->setVisible(false);
		}
	} else {
	
	}
}

void TVPMainScene::popAllUIForm() {
	auto children = UINode->getChildren();
	///for (auto ui : children) {
	for (auto p_ui = children.begin(); p_ui != children.end(); ++p_ui)
	{
		auto& ui = *p_ui;
		Size size = getContentSize();
		cocos2d::Node *ColorMask = MaskLayer::create(Color4B(0, 0, 0, 128), size.width, size.height);
		ColorMask->setPosition(Vec2(-size.width, 0));
		ui->addChild(ColorMask);
		ColorMask->runAction(FadeOut::create(UI_CHANGE_DURATION));
		ui->runAction(EaseQuadraticActionOut::create(MoveTo::create(UI_CHANGE_DURATION, Vec2(size.width, 0))));
		runAction(Sequence::createWithTwoActions(DelayTime::create(UI_CHANGE_DURATION), CallFunc::create([=](){
			ui->removeFromParent();
		})));
	}
}

void TVPMainScene::toggleVirtualMouseCursor() {
	showVirtualMouseCursor(!_mouseCursor || !_mouseCursor->isVisible());
}


void TVPMainScene::showVirtualMouseCursor(bool bVisible) {
	if (!bVisible) {
		if (_mouseCursor) _mouseCursor->setVisible(false);
		_virutalMouseMode = bVisible;
		return;
	}
	if (!_mouseCursor) {

	}
	_virutalMouseMode = bVisible;
}

bool TVPMainScene::isVirtualMouseMode() const {
	return _mouseCursor && _mouseCursor->isVisible();
}

bool TVPMainScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	return false;
}

void TVPMainScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {

}

void TVPMainScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {

}

void TVPMainScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {

}

bool TVPMainScene::attachWithIME()
{
	bool ret = IMEDelegate::attachWithIME();
	if (ret)
	{
		// open keyboard
		auto pGlView = Director::getInstance()->getOpenGLView();
		if (pGlView)
		{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WP8 && CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
			pGlView->setIMEKeyboardState(true);
#else
			pGlView->setIMEKeyboardState(true, "");
#endif
		}
	}
	return ret;
}

bool TVPMainScene::detachWithIME()
{
	bool ret = IMEDelegate::detachWithIME();
	if (ret)
	{
		// close keyboard
		auto glView = Director::getInstance()->getOpenGLView();
		if (glView)
		{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WP8 && CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
			glView->setIMEKeyboardState(false);
#else
			glView->setIMEKeyboardState(false, "");
#endif
		}
	}
	return ret;
}

bool TVPMainScene::canAttachWithIME()
{
	return true;
}

bool TVPMainScene::canDetachWithIME()
{
	return true;
}

void TVPMainScene::deleteBackward()
{

}

void TVPMainScene::insertText(const char * text, size_t len)
{
	std::string utf8(text, len);
	onTextInput(utf8);
}

void TVPMainScene::onCharInput(int keyCode) {

}

void TVPMainScene::onTextInput(const std::string &text) {

}

void TVPMainScene::onAxisEvent(cocos2d::Controller* ctrl, int keyCode, cocos2d::Event *e) {

}

void TVPMainScene::onPadKeyDown(cocos2d::Controller* ctrl, int keyCode, cocos2d::Event *e) {

}

void TVPMainScene::onPadKeyUp(cocos2d::Controller* ctrl, int keyCode, cocos2d::Event *e) {

}

void TVPMainScene::onPadKeyRepeat(cocos2d::Controller* ctrl, int code, cocos2d::Event *e) {

}

float TVPMainScene::convertCursorScale(float val/*0 ~ 1*/) {
	if (val <= 0.5f) {
		return 0.25f + (val * 2) * 0.75f;
	} else {
		return 1.f + (val - 0.5f) * 2.f;
	}
}
