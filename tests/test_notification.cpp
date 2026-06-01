#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include "Notification.h"

TEST_CASE("Notification: constructor works", "[notification]") {
    Notification n(1, 10, 100, "Hello");
    REQUIRE(n.getUserId() == 10);
    REQUIRE(n.getTaskId() == 100);
    REQUIRE(n.getMessage() == "Hello");
}

TEST_CASE("Notification: send does not throw", "[notification]") {
    Notification n(2, 20, 200, "Test");
    REQUIRE_NOTHROW(n.send());
}

TEST_CASE("Notification: markAsRead does not throw", "[notification]") {
    Notification n(3, 30, 300, "Msg");
    REQUIRE_NOTHROW(n.markAsRead());
}

TEST_CASE("Notification: addNotification stores in DB", "[notification][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "notif_add_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task task(0, uid, "Notif task", "2025-12-31");
    db.addTask(task);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    Notification n(0, uid, tid, "DB notification");
    REQUIRE(db.addNotification(n) == true);
}

TEST_CASE("Notification: multiple notifications can be added", "[notification][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "notif_multi_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task task(0, uid, "Multi task", "2025-12-31");
    db.addTask(task);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    Notification n1(0, uid, tid, "First");
    Notification n2(0, uid, tid, "Second");
    REQUIRE(db.addNotification(n1) == true);
    REQUIRE(db.addNotification(n2) == true);
}