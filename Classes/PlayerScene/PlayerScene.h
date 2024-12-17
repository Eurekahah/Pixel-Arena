#pragma once
#ifndef __PLAYER_SCENE_H__
#define __PLAYER_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

class PlayerScene : public cocos2d::Scene {
private:
    Player _player; // 玩家对象
    cocos2d::Label* _statusLabel; // 玩家状态显示
    cocos2d::MenuItemLabel* _matchButton; // 匹配按钮

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(PlayerScene);

    void onMatchButtonClicked(); // 匹配按钮点击回调
    void updateStatusLabel();    // 更新状态显示
    void onMatchSuccess();       // 匹配成功回调
};

#endif // __PLAYER_SCENE_H__
