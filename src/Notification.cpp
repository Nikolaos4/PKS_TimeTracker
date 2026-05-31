#include "Notification.h"
#include <iostream>

Notification::Notification(int id, int userId, int taskId, const std::string& message)
    : id(id), userId(userId), taskId(taskId), message(message), isRead(false) {}

void Notification::send() {
    // Здесь может быть реальная отправка уведомления, но пока оставим заглушку
    // В данной версии приложения уведомления не отправляются автоматически
}

void Notification::markAsRead() {
    isRead = true;
    // Логика пометки прочитанным
}