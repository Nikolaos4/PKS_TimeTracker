#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <vector>
#include <memory>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& event, int taskId, const std::string& message) = 0;
};

class Subject {
    std::vector<std::shared_ptr<Observer>> observers;
public:
    void attach(std::shared_ptr<Observer> obs);
    void detach(std::shared_ptr<Observer> obs);
    void notify(const std::string& event, int taskId, const std::string& message);
};

#endif