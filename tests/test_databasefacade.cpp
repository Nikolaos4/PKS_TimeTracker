#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"

TEST_CASE("DatabaseFacade registerUser returns true", "[db]") {
    DatabaseFacade db;
    REQUIRE(db.registerUser("test_user", "hash123") == true);
}

TEST_CASE("DatabaseFacade authenticateUser returns true", "[db]") {
    DatabaseFacade db;
    REQUIRE(db.authenticateUser("test_user", "hash123") == true);
}

TEST_CASE("DatabaseFacade addTask returns true", "[db]") {
    DatabaseFacade db;
    Task task(1, 1, "Test task", "2025-12-31");
    REQUIRE(db.addTask(task) == true);
}

TEST_CASE("DatabaseFacade getTasksForUser returns vector (maybe empty)", "[db]") {
    DatabaseFacade db;
    std::vector<Task> tasks = db.getTasksForUser(1);
    REQUIRE(tasks.size() == 0); // заглушка возвращает пустой вектор
}

TEST_CASE("DatabaseFacade updateTaskStatus returns true", "[db]") {
    DatabaseFacade db;
    REQUIRE(db.updateTaskStatus(1, "Completed") == true);
}

TEST_CASE("DatabaseFacade startTimer and stopTimer return true", "[db]") {
    DatabaseFacade db;
    REQUIRE(db.startTimer(1, "2025-05-17 12:00:00") == true);
    REQUIRE(db.stopTimer(1, "2025-05-17 14:00:00", 7200) == true);
}

TEST_CASE("DatabaseFacade addNotification returns true", "[db]") {
    DatabaseFacade db;
    Notification notif(1, 1, 1, "Test notify");
    REQUIRE(db.addNotification(notif) == true);
}

TEST_CASE("DatabaseFacade generateReport returns true", "[db]") {
    DatabaseFacade db;
    REQUIRE(db.generateReport(1) == true);
}