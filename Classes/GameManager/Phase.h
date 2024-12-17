#pragma once
#ifndef __PHASE_H__
#define __PHASE_H__

#include <chrono>
#include <algorithm>

class Phase {
public:
    using TimePoint = std::chrono::steady_clock::time_point;
    enum class PhaseName {
        PREPARING,          // ׼���׶�
        FIGHTING,           // ս���׶�
        WAITING,            // �ȴ��׶�
        FARMING             // Ұ�ֽ׶�

    };

private:
    PhaseName _phaseName;              // �׶�����
    double _duration;                   // �׶γ���ʱ�䣨�룩
    TimePoint _startTime;              // �׶ο�ʼʱ��

public:
    Phase(PhaseName phaseName, double duration)
        : _phaseName(phaseName), _duration(duration), _startTime(TimePoint()) {}

    void start();
    void end();
    bool isComplete() const;

    // ��ȡʣ��ʱ��
    double getTimeRemaining() const;

    // ������У��ʱ��
    void correctTimer(double serverTime);

    // ��ȡ�׶��Ѷȹ�ʱ��
    inline double Phase::getElapsed() const;
};

#endif // __PHASE_H__
