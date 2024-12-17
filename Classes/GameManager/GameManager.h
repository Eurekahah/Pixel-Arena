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
    std::string _gameId;               // 游戏 ID
    std::vector<std::shared_ptr<Round>> _rounds;
    int _currentRoundIndex;            // 当前回合索引
    cocos2d::Scheduler* _scheduler;   // 计时器依赖 Cocos2d 的 Scheduler

public:
    GameManager(const std::string& gameId);
    ~GameManager();

    void addRound(const std::shared_ptr<Round>& round);
    void startGame();
    void startRound();
    void nextRound();

    // 与服务器同步计时器
    void synchronizeWithServer(float serverTime);

    // 获取游戏状态
    int getCurrentRoundIndex() const { return _currentRoundIndex; }
    std::string getGameId() const { return _gameId; }
};

#endif // __GAMEMANAGER_H__
