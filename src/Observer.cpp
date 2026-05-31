#include "Observer.h"
#include <algorithm>

void Subject::attach(std::shared_ptr<Observer> obs) {
    observers.push_back(obs);
}

void Subject::detach(std::shared_ptr<Observer> obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
}

void Subject::notify(const std::string& event, int taskId, const std::string& message) {
    for (auto& obs : observers) {
        obs->update(event, taskId, message);
    }
}