#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include "Task.h"
#include <ctime>

TEST_CASE("DatabaseFacade: register and authenticate", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "int_" + std::to_string(rand());
    std::string pass = "pass";
    REQUIRE(db.registerUser(login, pass) == true);
    REQUIRE(db.authenticateUser(login, pass) == true);
    REQUIRE(db.authenticateUser(login, "wrong") == false);
}

TEST_CASE("DatabaseFacade: getUserIdByLogin returns -1 for unknown", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    REQUIRE(db.getUserIdByLogin("no_such_user_xyz") == -1);
}

TEST_CASE("DatabaseFacade: addTask and getTasksForUser", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "task_flow_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t1(0, uid, "Task1", "2025-01-01", "Desc1", 2);
    Task t2(0, uid, "Task2", "2025-01-02", "Desc2", 3);
    REQUIRE(db.addTask(t1) == true);
    REQUIRE(db.addTask(t2) == true);
    auto tasks = db.getTasksForUser(uid);
    REQUIRE(tasks.size() >= 2);
}

TEST_CASE("DatabaseFacade: updateTaskStatus", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "status_flow_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "Status Task", "2025-12-31");
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    REQUIRE(db.updateTaskStatus(tid, "Completed") == true);
}

TEST_CASE("DatabaseFacade: start and stop timer", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "timer_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "Timer task", "2025-12-31");
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string start(buf);
    REQUIRE(db.startTimer(tid, start) == true);
    now += 3600;
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string end(buf);
    REQUIRE(db.stopTimer(tid, end, 0) == true);
}

TEST_CASE("DatabaseFacade: editTask", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "edit_flow_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "Old", "2025-01-01", "Old desc", 1);
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    REQUIRE(db.editTask(tid, "New", "2026-01-01", "Completed", "New desc", 3) == true);
}

TEST_CASE("DatabaseFacade: deleteTask", "[db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "delete_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task t(0, uid, "To delete", "2025-12-31");
    db.addTask(t);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    REQUIRE(db.deleteTask(tid) == true);
    auto after = db.getTasksForUser(uid);
    bool found = false;
    for (const auto& tt : after) if (tt.getId() == tid) found = true;
    REQUIRE(found == false);
}