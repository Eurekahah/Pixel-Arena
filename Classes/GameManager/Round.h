#pragma once
#ifndef __ROUND_H__
#define __ROUND_H__

#include <vector>
#include <memory>
#include "Phase.h"

class Round {
private:
    std::string _roundId;                  // 回合编号
    std::vector<std::shared_ptr<Phase>> _phases;
    int _currentPhaseIndex;                // 当前阶段索引

public:
    Round::Round(std::string roundId)
        : _roundId(roundId), _currentPhaseIndex(0) {}


    void addPhase(const std::shared_ptr<Phase>& phase);
    void startPhase();
    void nextPhase();
    bool isComplete() const;

    int getCurrentPhaseIndex() const { return _currentPhaseIndex; }

    // 启动回合
    void start();

    // 结束回合
    void end();
};

#endif __ROUND_H__