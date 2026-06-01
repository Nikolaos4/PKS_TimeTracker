#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include "Task.h"

TEST_CASE("Task: constructor initializes fields", "[task]") {
    Task t(1, 100, "Title", "2025-12-31", "Desc", 2);
    REQUIRE(t.getId() == 1);
    REQUIRE(t.getUserId() == 100);
    REQUIRE(t.getTitle() == "Title");
    REQUIRE(t.getDeadline() == "2025-12-31");
    REQUIRE(t.getDescription() == "Desc");
    REQUIRE(t.getPriority() == 2);
    REQUIRE(t.getStatus() == "Pending");
}

TEST_CASE("Task: setStatus changes status", "[task]") {
    Task t(1, 1, "Test", "2025-12-31");
    t.setStatus("InProgress");
    REQUIRE(t.getStatus() == "InProgress");
    t.setStatus("Completed");
    REQUIRE(t.getStatus() == "Completed");
}

TEST_CASE("Task: default priority is 1", "[task]") {
    Task t(1, 1, "Test", "2025-12-31");
    REQUIRE(t.getPriority() == 1);
}

TEST_CASE("Task: adding task to database works", "[task][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "task_test_" + std::to_string(rand());
    db.registerUser(login, "pass");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "DB Task", "2026-01-01", "Test desc", 3);
    REQUIRE(db.addTask(t) == true);
}

TEST_CASE("Task: getTasksForUser returns tasks for specific user", "[task][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login1 = "userA_" + std::to_string(rand());
    std::string login2 = "userB_" + std::to_string(rand());
    db.registerUser(login1, "p");
    db.registerUser(login2, "p");
    int uid1 = db.getUserIdByLogin(login1);
    int uid2 = db.getUserIdByLogin(login2);
    Task t1(0, uid1, "Task A1", "2025-01-01");
    Task t2(0, uid1, "Task A2", "2025-01-02");
    Task t3(0, uid2, "Task B1", "2025-01-01");
    db.addTask(t1);
    db.addTask(t2);
    db.addTask(t3);
    auto tasksA = db.getTasksForUser(uid1);
    auto tasksB = db.getTasksForUser(uid2);
    REQUIRE(tasksA.size() >= 2);
    REQUIRE(tasksB.size() >= 1);
}

TEST_CASE("Task: updateTaskStatus changes status in DB", "[task][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "status_test_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "Status Task", "2025-12-31");
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    REQUIRE(db.updateTaskStatus(tid, "Completed") == true);
    auto updated = db.getTasksForUser(uid);
    for (const auto& task : updated) {
        if (task.getId() == tid) REQUIRE(task.getStatus() == "Completed");
    }
}

TEST_CASE("Task: editTask modifies fields", "[task][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "edit_test_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "Old title", "2025-01-01", "Old desc", 1);
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    REQUIRE(db.editTask(tid, "New title", "2026-01-01", "InProgress", "New desc", 3) == true);
    auto after = db.getTasksForUser(uid);
    for (const auto& task : after) {
        if (task.getId() == tid) {
            REQUIRE(task.getTitle() == "New title");
            REQUIRE(task.getDeadline() == "2026-01-01");
            REQUIRE(task.getStatus() == "InProgress");
            REQUIRE(task.getDescription() == "New desc");
            REQUIRE(task.getPriority() == 3);
        }
    }
}