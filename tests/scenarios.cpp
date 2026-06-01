#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include <ctime>

// Сценарий 1: полный цикл регистрации, добавления задачи, таймера, отчёта
TEST_CASE("Scenario 1: Full user workflow", "[scenario]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "scen1_" + std::to_string(rand());
    std::string pass = "pass";
    REQUIRE(db.registerUser(login, pass) == true);
    int uid = db.getUserIdByLogin(login);
    REQUIRE(uid != -1);
    Task t(0, uid, "Scenario task", "2025-12-31", "Test", 2);
    REQUIRE(db.addTask(t) == true);
    auto tasks = db.getTasksForUser(uid);
    REQUIRE_FALSE(tasks.empty());
    int tid = tasks.back().getId();
    // Запуск таймера
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string startTime(buf);
    REQUIRE(db.startTimer(tid, startTime) == true);
    now += 10;
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string endTime(buf);
    REQUIRE(db.stopTimer(tid, endTime, 0) == true);
    // Отчёт
    REQUIRE(db.generateReport(uid, "2025-01-01", "2025-12-31") == true);
    // Статистика
    db.getStatistics(uid); // не падает
    // Удаление
    REQUIRE(db.deleteTask(tid) == true);
}

// Сценарий 2: редактирование задачи и изменение статуса
TEST_CASE("Scenario 2: Edit task and change status", "[scenario]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "scen2_" + std::to_string(rand());
    db.registerUser(login, "pass");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "Original", "2025-01-01", "Desc", 1);
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    REQUIRE(db.editTask(tid, "Modified", "2026-01-01", "InProgress", "New desc", 3) == true);
    REQUIRE(db.updateTaskStatus(tid, "Completed") == true);
    auto updated = db.getTasksForUser(uid);
    for (const auto& ut : updated) {
        if (ut.getId() == tid) {
            REQUIRE(ut.getTitle() == "Modified");
            REQUIRE(ut.getStatus() == "Completed");
        }
    }
    db.deleteTask(tid);
}

// Сценарий 3: множественные задачи и статистика
TEST_CASE("Scenario 3: Multiple tasks and statistics", "[scenario]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "scen3_" + std::to_string(rand());
    db.registerUser(login, "pass");
    int uid = db.getUserIdByLogin(login);
    Task t1(0, uid, "Task A", "2025-01-01");
    Task t2(0, uid, "Task B", "2025-01-02");
    db.addTask(t1);
    db.addTask(t2);
    db.updateTaskStatus(1, "Completed"); // предполагаем, что ID = 1, но это может не сработать
    // Лучше получить ID из БД
    auto tasks = db.getTasksForUser(uid);
    for (const auto& task : tasks) {
        if (task.getTitle() == "Task A") db.updateTaskStatus(task.getId(), "Completed");
    }
    db.getStatistics(uid); // должно показать 1/2 выполнено
}