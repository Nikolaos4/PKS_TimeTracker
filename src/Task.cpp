#include "Task.h"
#include <iostream>

Task::Task(int id, int userId, const std::string& title, const std::string& deadline,
           const std::string& description, int priority)
    : id(id), userId(userId), title(title), deadline(deadline), status("Pending"),
      description(description), priority(priority) {}

int Task::getId() const { return id; }
int Task::getUserId() const { return userId; }
std::string Task::getTitle() const { return title; }
std::string Task::getDeadline() const { return deadline; }
std::string Task::getStatus() const { return status; }
void Task::setStatus(const std::string& newStatus) { status = newStatus; }

std::string Task::getDescription() const { return description; }
void Task::setDescription(const std::string& desc) { description = desc; }
int Task::getPriority() const { return priority; }
void Task::setPriority(int prio) { priority = prio; }

// Заглушки таймера (реальная логика в DatabaseFacade)
void Task::startTimer() {}
void Task::stopTimer(int) {}