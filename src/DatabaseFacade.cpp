#include "DatabaseFacade.h"
#include <iostream>

static DatabaseFacade* instance = nullptr;

DatabaseFacade& DatabaseFacade::getInstance() {
    if (!instance) {
        instance = new DatabaseFacade();
    }
    return *instance;
}

bool DatabaseFacade::registerUser(const std::string& login, const std::string& passwordHash) {
    std::cout << "Фасад: регистрация пользователя " << login << std::endl;
    return true;
}

bool DatabaseFacade::authenticateUser(const std::string& login, const std::string& passwordHash) {
    std::cout << "Фасад: аутентификация " << login << std::endl;
    return true;
}

bool DatabaseFacade::addTask(const Task& task) {
    std::cout << "Фасад: сохранение задачи #" << task.getId() << std::endl;
    return true;
}

std::vector<Task> DatabaseFacade::getTasksForUser(int userId) {
    std::cout << "Фасад: получение задач для пользователя " << userId << std::endl;
    return {}; // пустой вектор
}

bool DatabaseFacade::updateTaskStatus(int taskId, const std::string& status) {
    std::cout << "Фасад: обновление статуса задачи " << taskId << " -> " << status << std::endl;
    return true;
}

bool DatabaseFacade::startTimer(int taskId, const std::string& startTime) {
    std::cout << "Фасад: запуск таймера для задачи " << taskId << " в " << startTime << std::endl;
    return true;
}

bool DatabaseFacade::stopTimer(int taskId, const std::string& endTime, int duration) {
    std::cout << "Фасад: остановка таймера задачи " << taskId
              << ", длительность " << duration << std::endl;
    return true;
}

bool DatabaseFacade::addNotification(const Notification& notif) {
    std::cout << "Фасад: сохранение уведомления" << std::endl;
    return true;
}

bool DatabaseFacade::generateReport(int userId) {
    std::cout << "Фасад: генерация отчёта для пользователя " << userId << std::endl;
    return true;
}