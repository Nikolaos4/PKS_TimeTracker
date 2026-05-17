#include <iostream>
#include "Task.h"
#include "User.h"
#include "TimeEntry.h"
#include "Notification.h"
#include "DatabaseFacade.h"

int main() {
    std::cout << "=== Тайм-трекер (заглушки) ===" << std::endl;

    auto& db = DatabaseFacade::getInstance();
    db.registerUser("student", "hash");
    db.authenticateUser("student", "hash");

    Task t(1, 1, "Курсовая", "2025-05-01");
    db.addTask(t);

    db.startTimer(1, "2025-04-26 10:00");
    db.stopTimer(1, "2025-04-26 12:30", 1500);

    Notification n(1, 1, 1, "Дедлайн приближается!");
    n.send();
    db.addNotification(n);

    db.generateReport(1);
    return 0;
}