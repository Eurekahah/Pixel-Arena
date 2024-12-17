#include "PlayerScene.h"
#include "NetworkManager.h"
#include "GameScene.h"

USING_NS_CC;

Scene* PlayerScene::createScene() {
    return PlayerScene::create();
}

bool PlayerScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 初始化玩家
    _player = Player("Test1");

    // 背景
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 状态显示
    _statusLabel = Label::createWithTTF("状态: 空闲中", "../../Resources/fonts/Marker Felt.ttf", 24);
    _statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - _statusLabel->getContentSize().height));
    this->addChild(_statusLabel, 1);

    // 匹配按钮
    auto matchLabel = Label::createWithTTF("开始匹配", "../../Resources/fonts/Marker Felt.ttf", 32);
    _matchButton = MenuItemLabel::create(matchLabel, CC_CALLBACK_0(PlayerScene::onMatchButtonClicked, this));
    _matchButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto menu = Menu::create(_matchButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    return true;
}

void PlayerScene::onMatchButtonClicked() {
    if (_player.getStatus() == Player::Status::IDLE) {
        // 更新状态
        _player.setStatus(Player::Status::MATCHING);
        updateStatusLabel();

        // 发送匹配请求到服务器
        auto message = "{\"action\": \"match_request\", \"player_id\": \"" + _player.getPlayerId() + "\"}";
        NetworkManager::getInstance()->sendMessage(message);
        log("已向服务器发送匹配请求...");

        // 这里模拟匹配成功
        this->scheduleOnce([this](float) {
            onMatchSuccess();
            }, 3.0f, "MatchSuccessCallback"); // 模拟延迟3秒匹配成功
    }
}

void PlayerScene::updateStatusLabel() {
    std::string statusStr;
    switch (_player.getStatus()) {
    case Player::Status::IDLE: statusStr = "状态: 空闲中"; break;
    case Player::Status::MATCHING: statusStr = "状态: 匹配中"; break;
    case Player::Status::PLAYING: statusStr = "状态: 游戏中"; break;
    }
    _statusLabel->setString(statusStr);
}

void PlayerScene::onMatchSuccess() {
    // 更新状态
    _player.setStatus(Player::Status::PLAYING);
    updateStatusLabel();

    // 进入游戏场景
    log("匹配成功，进入游戏开始场景...");
    auto gameScene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}
