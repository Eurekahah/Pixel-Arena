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

    // ��ʼ�����
    _player = Player("Test1");

    // ����
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ״̬��ʾ
    _statusLabel = Label::createWithTTF("״̬: ������", "../../Resources/fonts/Marker Felt.ttf", 24);
    _statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - _statusLabel->getContentSize().height));
    this->addChild(_statusLabel, 1);

    // ƥ�䰴ť
    auto matchLabel = Label::createWithTTF("��ʼƥ��", "../../Resources/fonts/Marker Felt.ttf", 32);
    _matchButton = MenuItemLabel::create(matchLabel, CC_CALLBACK_0(PlayerScene::onMatchButtonClicked, this));
    _matchButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto menu = Menu::create(_matchButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    return true;
}

void PlayerScene::onMatchButtonClicked() {
    if (_player.getStatus() == Player::Status::IDLE) {
        // ����״̬
        _player.setStatus(Player::Status::MATCHING);
        updateStatusLabel();

        // ����ƥ�����󵽷�����
        auto message = "{\"action\": \"match_request\", \"player_id\": \"" + _player.getPlayerId() + "\"}";
        NetworkManager::getInstance()->sendMessage(message);
        log("�������������ƥ������...");

        // ����ģ��ƥ��ɹ�
        this->scheduleOnce([this](float) {
            onMatchSuccess();
            }, 3.0f, "MatchSuccessCallback"); // ģ���ӳ�3��ƥ��ɹ�
    }
}

void PlayerScene::updateStatusLabel() {
    std::string statusStr;
    switch (_player.getStatus()) {
    case Player::Status::IDLE: statusStr = "״̬: ������"; break;
    case Player::Status::MATCHING: statusStr = "״̬: ƥ����"; break;
    case Player::Status::PLAYING: statusStr = "״̬: ��Ϸ��"; break;
    }
    _statusLabel->setString(statusStr);
}

void PlayerScene::onMatchSuccess() {
    // ����״̬
    _player.setStatus(Player::Status::PLAYING);
    updateStatusLabel();

    // ������Ϸ����
    log("ƥ��ɹ���������Ϸ��ʼ����...");
    auto gameScene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
}
