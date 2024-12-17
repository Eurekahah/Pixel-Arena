#pragma once
#include "cocos2d.h"
#include "GameManager.h"

class GameScene : public cocos2d::Scene {
private:
    GameManager _gameManager;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);

    void update(float dt) override; // 用于计时器更新
};
