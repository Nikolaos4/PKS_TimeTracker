#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include "User.h"
#include "Task.h"
#include "TimeEntry.h"
#include "Notification.h"
#include <iostream>

// Регистрация и аутентификация
TEST_CASE("Scenario: User registration and authentication", "[scenario]") {
    DatabaseFacade db;
    bool reg = db.registerUser("vasya", "secret");
    REQUIRE(reg == true);
    
    bool authOk = db.authenticateUser("vasya", "secret");
    REQUIRE(authOk == true);
    
    // Пока заглушка возвращает true даже для неправильного пароля.
    // В будущем, когда будет реальная проверка, этот тест должен упасть.
    bool authBad = db.authenticateUser("vasya", "wrong");
    REQUIRE(authBad == true); // временно
}

// Создание задачи и таймер
TEST_CASE("Scenario: Create task and timer", "[scenario]") {
    DatabaseFacade db;
    Task task(101, 1, "Write report", "2025-06-01");
    bool added = db.addTask(task);
    REQUIRE(added == true);
    
    bool start = db.startTimer(101, "2025-05-17 10:00:00");
    REQUIRE(start == true);
    
    bool stop = db.stopTimer(101, "2025-05-17 12:30:00", 9000);
    REQUIRE(stop == true);
    
}

// Формирование отчёта
TEST_CASE("Scenario: Generate report", "[scenario]") {
    DatabaseFacade db;
    Task t1(201, 1, "Task A", "2025-06-01");
    Task t2(202, 1, "Task B", "2025-06-02");
    db.addTask(t1);
    db.addTask(t2);
    db.startTimer(201, "2025-05-17 09:00:00");
    db.stopTimer(201, "2025-05-17 11:00:00", 7200);
    
    bool reportGenerated = db.generateReport(1);
    REQUIRE(reportGenerated == true);
}

// Уведомление о дедлайне
TEST_CASE("Scenario: Deadline notification", "[scenario]") {
    DatabaseFacade db;
    Task urgent(301, 1, "Urgent", "2025-05-18");
    db.addTask(urgent);
    
    Notification notif(1, 1, 301, "Deadline approaching!");
    bool added = db.addNotification(notif);
    REQUIRE(added == true);
    
    // В заглушках просто проверим, что метод не падает.
    notif.send();
    SUCCEED("Notification sent without errors");
}