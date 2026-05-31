#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
private:
    int id;
    int userId;
    int taskId;
    std::string message;
    bool isRead;
public:
    Notification(int id, int userId, int taskId, const std::string& message);
    void send();
    void markAsRead();
    int getUserId() const { return userId; }
    int getTaskId() const { return taskId; }
    std::string getMessage() const { return message; }
};

#endif