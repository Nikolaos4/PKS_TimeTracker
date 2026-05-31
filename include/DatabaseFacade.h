#ifndef DATABASEFACADE_H
#define DATABASEFACADE_H

#include <vector>
#include <string>
#include "Task.h"
#include "User.h"
#include "TimeEntry.h"
#include "Notification.h"
#include "Observer.h"

namespace pqxx {
    class connection;
}

class DatabaseFacade : public Subject {
public:
    static DatabaseFacade& getInstance();
    ~DatabaseFacade();

    bool registerUser(const std::string& login, const std::string& passwordHash);
    bool authenticateUser(const std::string& login, const std::string& passwordHash);
    bool addTask(const Task& task);
    std::vector<Task> getTasksForUser(int userId);
    bool updateTaskStatus(int taskId, const std::string& status);
    bool startTimer(int taskId, const std::string& startTime);
    bool stopTimer(int taskId, const std::string& endTime, int duration);
    bool addNotification(const Notification& notif);
    bool generateReport(int userId);
    int getUserIdByLogin(const std::string& login);
    bool editTask(int taskId, const std::string& newTitle, const std::string& newDeadline,
                  const std::string& newStatus, const std::string& newDescription, int newPriority);
    bool deleteTask(int taskId);
    void getStatistics(int userId);

private:
    DatabaseFacade();
    pqxx::connection* conn;
};

#endif