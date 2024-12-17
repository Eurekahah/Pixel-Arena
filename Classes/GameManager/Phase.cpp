#include "Phase.h"

void Phase::start() {
    _startTime = std::chrono::steady_clock::now();
}

void Phase::end() {
    // �׶ν����߼�������չ
}

bool Phase::isComplete() const {
    return getElapsed() >= _duration;
}

double Phase::getTimeRemaining() const {
    return std::max(0.0, _duration - getElapsed());
}

void Phase::correctTimer(double serverTime) {
    // ʹ�÷�������ʣ��ʱ��У�����ؼ�ʱ��
    double remaining = getTimeRemaining();
    double offset = serverTime - remaining;
    _startTime += std::chrono::duration<double>(offset);
}

// ��ȡ����ʱ�䣨�룩
inline double Phase::getElapsed() const {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - _startTime).count();
}