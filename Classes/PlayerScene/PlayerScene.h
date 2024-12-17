#pragma once
#ifndef __PLAYER_SCENE_H__
#define __PLAYER_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

class PlayerScene : public cocos2d::Scene {
private:
    Player _player; // ��Ҷ���
    cocos2d::Label* _statusLabel; // ���״̬��ʾ
    cocos2d::MenuItemLabel* _matchButton; // ƥ�䰴ť

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(PlayerScene);

    void onMatchButtonClicked(); // ƥ�䰴ť����ص�
    void updateStatusLabel();    // ����״̬��ʾ
    void onMatchSuccess();       // ƥ��ɹ��ص�
};

#endif // __PLAYER_SCENE_H__
