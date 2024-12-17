#pragma once
#ifndef __PHASE_H__
#define __PHASE_H__

#include <chrono>
#include <algorithm>

class Phase {
public:
    using TimePoint = std::chrono::steady_clock::time_point;
    enum class PhaseName {
        PREPARING,          // 准备阶段
        FIGHTING,           // 战斗阶段
        WAITING,            // 等待阶段
        FARMING             // 野怪阶段

    };

private:
    PhaseName _phaseName;              // 阶段名称
    double _duration;                   // 阶段持续时间（秒）
    TimePoint _startTime;              // 阶段开始时间

public:
    Phase(PhaseName phaseName, double duration)
        : _phaseName(phaseName), _duration(duration), _startTime(TimePoint()) {}

    void start();
    void end();
    bool isComplete() const;

    // 获取剩余时间
    double getTimeRemaining() const;

    // 服务器校正时间
    void correctTimer(double serverTime);

    // 获取阶段已度过时间
    inline double Phase::getElapsed() const;
};

#endif // __PHASE_H__
