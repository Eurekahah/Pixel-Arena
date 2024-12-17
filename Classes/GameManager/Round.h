#pragma once
#ifndef __ROUND_H__
#define __ROUND_H__

#include <vector>
#include <memory>
#include "Phase.h"

class Round {
private:
    std::string _roundId;                  // �غϱ��
    std::vector<std::shared_ptr<Phase>> _phases;
    int _currentPhaseIndex;                // ��ǰ�׶�����

public:
    Round::Round(std::string roundId)
        : _roundId(roundId), _currentPhaseIndex(0) {}


    void addPhase(const std::shared_ptr<Phase>& phase);
    void startPhase();
    void nextPhase();
    bool isComplete() const;

    int getCurrentPhaseIndex() const { return _currentPhaseIndex; }

    // �����غ�
    void start();

    // �����غ�
    void end();
};

#endif __ROUND_H__