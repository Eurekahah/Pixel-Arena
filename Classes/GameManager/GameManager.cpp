#include "GameManager.h"


GameManager::GameManager(const std::string& gameId)
    : _gameId(gameId), _currentRoundIndex(0), _scheduler(cocos2d::Director::getInstance()->getScheduler()) {}

GameManager::~GameManager() {
    // 清理资源或计时器
}

void GameManager::addRound(const std::shared_ptr<Round>& round) {
    _rounds.push_back(round);
}

void GameManager::startGame() {
    if (!_rounds.empty()) {
        _currentRoundIndex = 0;
        startRound();
    }
}

void GameManager::startRound() {
    if (_currentRoundIndex < _rounds.size()) {
        _rounds[_currentRoundIndex]->startPhase();
    }
}

void GameManager::nextRound() {
    if (_currentRoundIndex < _rounds.size()) {
        _rounds[_currentRoundIndex]->nextPhase();
        if (_rounds[_currentRoundIndex]->isComplete()) {
            _currentRoundIndex++;
            startRound();
        }
    }
}

void GameManager::synchronizeWithServer(float serverTime) {
    if (_currentRoundIndex < _rounds.size()) {
        auto& currentRound = _rounds[_currentRoundIndex];
        if (currentRound->getCurrentPhaseIndex() < currentRound->_phases.size()) {
            currentRound->_phases[currentRound->getCurrentPhaseIndex()]->correctTimer(serverTime);
        }
    }
}
