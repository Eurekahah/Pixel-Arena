#include "Round.h"


void Round::addPhase(const std::shared_ptr<Phase>& phase) {
    _phases.push_back(std::make_unique<Phase>(phase));
}

void Round::startPhase() {
    if (_currentPhaseIndex < _phases.size()) [[likely]] {
        _phases[_currentPhaseIndex]->start();
    }
}

void Round::nextPhase() {
    if (_currentPhaseIndex < _phases.size()) [[likely]] {
        _phases[_currentPhaseIndex]->end();
        _currentPhaseIndex++;
        if (_currentPhaseIndex < _phases.size()) [[likely]] {
            _phases[_currentPhaseIndex]->start();
        }
    }
}

bool Round::isComplete() const {
    return _currentPhaseIndex >= _phases.size();
}

void Round::start() {
    if (!_phases.empty()) {
        startPhase();
    }
    else {
        throw std::runtime_error("No phases available to start.");
    }
}

void Round::end() {
    while (!isComplete()) {
        nextPhase();
    }
}
