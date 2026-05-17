#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

class Task {
private:
    int id;
    int userId;
    std::string title;
    std::string deadline;
    std::string status; // Pending, InProgress, Completed
public:
    Task(int id, int userId, const std::string& title, const std::string& deadline);
    void setStatus(const std::string& newStatus);
    std::string getStatus() const;
    int getId() const;
    int getUserId() const;
    void startTimer();
    void stopTimer(int durationSeconds);
};

#endif