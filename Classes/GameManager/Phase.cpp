#include "Phase.h"

void Phase::start() {
    _startTime = std::chrono::steady_clock::now();
}

void Phase::end() {
    // 阶段结束逻辑可以扩展
}

bool Phase::isComplete() const {
    return getElapsed() >= _duration;
}

double Phase::getTimeRemaining() const {
    return std::max(0.0, _duration - getElapsed());
}

void Phase::correctTimer(double serverTime) {
    // 使用服务器的剩余时间校正本地计时器
    double remaining = getTimeRemaining();
    double offset = serverTime - remaining;
    _startTime += std::chrono::duration<double>(offset);
}

// 获取已用时间（秒）
inline double Phase::getElapsed() const {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - _startTime).count();
}