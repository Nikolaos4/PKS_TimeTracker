#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
private:
    int id;
    int userId;
    std::string title;
    std::string deadline;
    std::string status;        // поле статуса
    std::string description;
    int priority;
public:
    Task(int id, int userId, const std::string& title, const std::string& deadline, 
         const std::string& description = "", int priority = 1);
    
    // Геттеры и сеттеры
    int getId() const;
    int getUserId() const;
    std::string getTitle() const;
    std::string getDeadline() const;
    std::string getStatus() const;
    void setStatus(const std::string& newStatus);
    
    std::string getDescription() const;
    void setDescription(const std::string& desc);
    int getPriority() const;
    void setPriority(int prio);
    
    // Методы таймера (можно оставить заглушками)
    void startTimer();
    void stopTimer(int durationSeconds);
};

#endif