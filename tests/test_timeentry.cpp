#include <catch2/catch_all.hpp>
#include "TimeEntry.h"

TEST_CASE("TimeEntry constructor initializes fields correctly", "[timeentry]") {
    TimeEntry te(10, 42, "2025-05-17 10:00:00");
    REQUIRE(te.getDuration() == 0);
}

TEST_CASE("TimeEntry stop sets duration correctly", "[timeentry]") {
    TimeEntry te(1, 100, "2025-05-17 09:00:00");
    te.stop("2025-05-17 11:30:00", 9000);
    REQUIRE(te.getDuration() == 9000);
}

TEST_CASE("TimeEntry stop can be called multiple times", "[timeentry]") {
    TimeEntry te(2, 200, "2025-05-17 08:00:00");
    te.stop("2025-05-17 09:00:00", 3600);
    REQUIRE(te.getDuration() == 3600);
    te.stop("2025-05-17 10:00:00", 7200);
    REQUIRE(te.getDuration() == 7200); // последний вызов перезаписывает
}

TEST_CASE("TimeEntry getDuration returns zero before stop", "[timeentry]") {
    TimeEntry te(3, 300, "2025-05-17 12:00:00");
    REQUIRE(te.getDuration() == 0);
}

TEST_CASE("TimeEntry different IDs and taskIds", "[timeentry]") {
    TimeEntry te1(1, 100, "2025-01-01 00:00:00");
    TimeEntry te2(2, 200, "2025-01-02 00:00:00");
    // Сравнивать нечего, но проверяем, что конструктор работает.
    REQUIRE(te1.getDuration() == 0);
    REQUIRE(te2.getDuration() == 0);
}