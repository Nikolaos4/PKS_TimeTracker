#include "Task.h"
#include <iostream>

Task::Task(int id, int userId, const std::string& title, const std::string& deadline)
    : id(id), userId(userId), title(title), deadline(deadline), status("Pending") {}

void Task::setStatus(const std::string& newStatus) {
    status = newStatus;
    std::cout << "Task #" << id << " статус изменён на " << status << std::endl;
}

std::string Task::getStatus() const { return status; }
int Task::getId() const { return id; }
int Task::getUserId() const { return userId; }

void Task::startTimer() {
    std::cout << "Task #" << id << " запуск таймера" << std::endl;
}

void Task::stopTimer(int durationSeconds) {
    std::cout << "Task #" << id << " остановка таймера, длительность: " << durationSeconds << " сек." << std::endl;
}
