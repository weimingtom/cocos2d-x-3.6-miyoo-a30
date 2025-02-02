#include "DebugViewLayerForm.h"
#include "extensions/GUI/CCScrollView/CCTableView.h"
//#include "WindowIntf.h"
//#include "DrawDevice.h"
#include "cocos2d/MainScene.h"
#include <ui/UIButton.h>
#include <2d/CCSprite.h>
#include <2d/CCLabel.h>
//#include "RenderManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class DebugViewLayerForm::DebugViewLayerCell : public TableViewCell {
public:
	static DebugViewLayerCell* create() {
		DebugViewLayerCell* ret = new DebugViewLayerCell;
		ret->autorelease();
		ret->init();
		return ret;
	}

	virtual bool init() override {
		TableViewCell::init();
		_bottomLine = LayerColor::create(Color4B::WHITE, 100, 1);
		addChild(_bottomLine);
		_sprite = Sprite::create();
		_sprite->setAnchorPoint(Vec2::ZERO);
		addChild(_sprite);
		_name = Label::createWithTTF("", "DroidSansFallback.ttf", 16);
		_name->setAnchorPoint(Vec2::ZERO);
		addChild(_name);
		_memsize = Label::createWithTTF("", "DroidSansFallback.ttf", 16);
		_memsize->setAnchorPoint(Vec2(1, 0));
		addChild(_memsize);
		return true;
	}

	virtual void setContentSize(const Size& contentSize) override {
		TableViewCell::setContentSize(contentSize);
		_bottomLine->setContentSize(Size(contentSize.width, 2));
	}

	void setData(const Size &laySize, const LayerInfo &data) {

	}

private:
	LayerColor *_bottomLine;
	Sprite *_sprite;
	Label *_name, *_memsize;
};

DebugViewLayerForm * DebugViewLayerForm::create() {
	DebugViewLayerForm * ret = new DebugViewLayerForm;
	ret->autorelease();
	ret->init();
	return ret;
}

class tHackTableView : public TableView {
public:
	void setSwallowTouches(bool swallow) {
		if (_touchListener) {
			_touchListener->setSwallowTouches(swallow);
		}
	}
};

static unsigned char _2x2_block_Image[] = {
	// RGBA8888
	0x2F, 0x2F, 0x2F, 0xFF,
	0x40, 0x40, 0x40, 0xFF,
	0x40, 0x40, 0x40, 0xFF,
	0x2F, 0x2F, 0x2F, 0xFF
};

bool DebugViewLayerForm::init() {
	Node::init();
	Size selfsize = TVPMainScene::GetInstance()->getGameNodeSize();
	setContentSize(selfsize);

	Texture2D* tex = new Texture2D();
	tex->autorelease();
	tex->initWithData(_2x2_block_Image, 16, Texture2D::PixelFormat::RGBA8888, 2, 2, Size::ZERO);
	Texture2D::TexParams param_ = {
		GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT
	};
	tex->setTexParameters(param_);
	Sprite *_backGround = Sprite::create();
	_backGround->setTexture(tex);
	_backGround->setScale(16);
	_backGround->setTextureRect(Rect(0, 0, selfsize.width / 16, selfsize.height / 16));
	//LayerColor *_backGround = LayerColor::create(Color4B(16, 16, 16, 255), selfsize.width, selfsize.height);
	_backGround->setAnchorPoint(Vec2::ZERO);
	addChild(_backGround);

	_tableView = TableView::create(this, getContentSize());
	_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	static_cast<tHackTableView*>(_tableView)->setSwallowTouches(true);
	addChild(_tableView);
	setOnExitCallback(std::bind(&DebugViewLayerForm::onExitCallback, this));

	_totalSize = Label::createWithTTF("", "DroidSansFallback.ttf", 16);
	_totalSize->setAnchorPoint(Vec2(1, 1));

	uint64_t totalSize = 0;
	char tmp[32];
	sprintf(tmp, "%.2fMB", (float)((double)totalSize / (1024.f * 1024.f)));
	_totalSize->setString(tmp);
	addChild(_totalSize);

	ui::Button *btnClose = ui::Button::create("img/Cancel_Normal.png", "img/Cancel_Press.png");
	btnClose->setTouchEnabled(true);
	btnClose->addClickEventListener([this](Ref*){
		removeFromParent();
	});
	btnClose->setPosition(getContentSize() - btnClose->getContentSize());
	addChild(btnClose);
	_totalSize->setPosition(btnClose->getPosition().x, getContentSize().height);

	_tableView->reloadData();
	return true;
}

Size DebugViewLayerForm::tableCellSizeForIndex(TableView *table, ssize_t idx) {
	cocos2d::Size laySize(getContentSize().width, 0);
	laySize.height += 24;
	return laySize;
}

cocos2d::extension::TableViewCell* DebugViewLayerForm::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) {
	cocos2d::Size laySize(getContentSize().width, 0);
	DebugViewLayerCell *cell = static_cast<DebugViewLayerCell*>(table->dequeueCell());
	if (!cell) cell = DebugViewLayerCell::create();

	cell->setData(laySize, _layers[idx]);
	return cell;
}

void DebugViewLayerForm::onExitCallback() {
	TVPMainScene::GetInstance()->scheduleUpdate();
}

