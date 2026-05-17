#ifndef DATABASEFACADE_H
#define DATABASEFACADE_H

#include <vector>
#include "Task.h"
#include "User.h"
#include "TimeEntry.h"
#include "Notification.h"

class DatabaseFacade {
public:
    static DatabaseFacade& getInstance(); 


    bool registerUser(const std::string& login, const std::string& passwordHash);
    bool authenticateUser(const std::string& login, const std::string& passwordHash);

    bool addTask(const Task& task);
    std::vector<Task> getTasksForUser(int userId);
    bool updateTaskStatus(int taskId, const std::string& status);

    bool startTimer(int taskId, const std::string& startTime);
    bool stopTimer(int taskId, const std::string& endTime, int duration);

    bool addNotification(const Notification& notif);

    bool generateReport(int userId);
};

#endif