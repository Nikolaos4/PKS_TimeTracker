#include "Notification.h"
#include <iostream>

Notification::Notification(int id, int userId, int taskId, const std::string& message)
    : id(id), userId(userId), taskId(taskId), message(message), isRead(false) {}

void Notification::send() {
    std::cout << "Уведомление для пользователя " << userId
              << " по задаче " << taskId << ": " << message << std::endl;
}

void Notification::markAsRead() {
    isRead = true;
    std::cout << "Уведомление #" << id << " отмечено прочитанным" << std::endl;
}