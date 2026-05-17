#include <catch2/catch_all.hpp>
#include "Task.h"

TEST_CASE("Task constructor initializes fields correctly", "[task]") {
    Task task(100, 42, "Write report", "2025-06-01");
    REQUIRE(task.getId() == 100);
    REQUIRE(task.getUserId() == 42);
    REQUIRE(task.getStatus() == "Pending");
}

TEST_CASE("Task status can be changed", "[task]") {
    Task task(1, 1, "Test", "2025-12-31");
    task.setStatus("InProgress");
    REQUIRE(task.getStatus() == "InProgress");
    task.setStatus("Completed");
    REQUIRE(task.getStatus() == "Completed");
}

TEST_CASE("Start timer does not throw", "[task]") {
    Task task(2, 1, "Coding", "2025-06-10");
    REQUIRE_NOTHROW(task.startTimer());
}

TEST_CASE("Stop timer does not throw", "[task]") {
    Task task(3, 1, "Debugging", "2025-06-15");
    REQUIRE_NOTHROW(task.stopTimer(3600));
}

TEST_CASE("Multiple timer calls do not throw", "[task]") {
    Task task(5, 1, "Refactoring", "2025-07-01");
    REQUIRE_NOTHROW(task.startTimer());
    REQUIRE_NOTHROW(task.stopTimer(120));
    REQUIRE_NOTHROW(task.startTimer());
    REQUIRE_NOTHROW(task.stopTimer(300));
}

TEST_CASE("Different tasks have different IDs and user IDs", "[task]") {
    Task t1(10, 100, "A", "2025-01-01");
    Task t2(20, 200, "B", "2025-01-02");
    REQUIRE(t1.getId() != t2.getId());
    REQUIRE(t1.getUserId() != t2.getUserId());
}