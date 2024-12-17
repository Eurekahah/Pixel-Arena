#pragma once
#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <string>
#include <vector>
#include <memory>
#include "Round.h"
#include "cocos2d.h"



class GameManager {
private:
    std::string _gameId;               // ��Ϸ ID
    std::vector<std::shared_ptr<Round>> _rounds;
    int _currentRoundIndex;            // ��ǰ�غ�����
    cocos2d::Scheduler* _scheduler;   // ��ʱ������ Cocos2d �� Scheduler

public:
    GameManager(const std::string& gameId);
    ~GameManager();

    void addRound(const std::shared_ptr<Round>& round);
    void startGame();
    void startRound();
    void nextRound();

    // �������ͬ����ʱ��
    void synchronizeWithServer(float serverTime);

    // ��ȡ��Ϸ״̬
    int getCurrentRoundIndex() const { return _currentRoundIndex; }
    std::string getGameId() const { return _gameId; }
};

#endif // __GAMEMANAGER_H__
