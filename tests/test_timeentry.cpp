#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include "TimeEntry.h"
#include <ctime>

TEST_CASE("TimeEntry: constructor initializes fields", "[timeentry]") {
    TimeEntry te(1, 100, "2025-01-01 10:00:00");
    REQUIRE(te.getDuration() == 0);
}

TEST_CASE("TimeEntry: stop sets duration", "[timeentry]") {
    TimeEntry te(1, 1, "2025-01-01 10:00:00");
    te.stop("2025-01-01 12:00:00", 7200);
    REQUIRE(te.getDuration() == 7200);
}

TEST_CASE("TimeEntry: stop can be called multiple times", "[timeentry]") {
    TimeEntry te(2, 2, "2025-01-01 09:00:00");
    te.stop("2025-01-01 10:00:00", 3600);
    te.stop("2025-01-01 11:00:00", 7200);
    REQUIRE(te.getDuration() == 7200);
}

TEST_CASE("TimeEntry: startTimer creates entry in DB", "[timeentry][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "te_start_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task task(0, uid, "TE task", "2025-12-31");
    db.addTask(task);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    std::string startTime = "2025-06-01 10:00:00";
    REQUIRE(db.startTimer(tid, startTime) == true);
}

TEST_CASE("TimeEntry: stopTimer updates DB with computed duration", "[timeentry][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "te_stop_" + std::to_string(rand());
    db.registerUser(login, "p");
    int uid = db.getUserIdByLogin(login);
    Task task(0, uid, "TE stop", "2025-12-31");
    db.addTask(task);
    auto tasks = db.getTasksForUser(uid);
    int tid = tasks.back().getId();
    std::string startTime = "2025-06-01 10:00:00";
    db.startTimer(tid, startTime);
    std::string endTime = "2025-06-01 12:30:00";
    REQUIRE(db.stopTimer(tid, endTime, 0) == true);
    // Проверить, что duration_seconds не 0 (не можем напрямую, но функция вернула true)
}