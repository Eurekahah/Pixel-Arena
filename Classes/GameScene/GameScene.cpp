#include "GameScene.h"

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    _gameManager = GameManager("Game1");
    _gameManager.startNewRound();

    this->scheduleUpdate();
    return true;
}

void GameScene::update(float dt) {
    _gameManager.update(dt);
    // ���½��棬��ʾ�׶κͻغ�״̬
}
