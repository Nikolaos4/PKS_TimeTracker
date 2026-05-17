#include <catch2/catch_all.hpp>
#include "Notification.h"

TEST_CASE("Notification constructor does not throw", "[notification]") {
    REQUIRE_NOTHROW(Notification(1, 10, 100, "Test message"));
}

TEST_CASE("Notification send does not throw", "[notification]") {
    Notification n(1, 10, 100, "Test");
    REQUIRE_NOTHROW(n.send());
}

TEST_CASE("Notification markAsRead does not throw", "[notification]") {
    Notification n(2, 20, 200, "Another");
    REQUIRE_NOTHROW(n.markAsRead());
}

TEST_CASE("Multiple operations on notification", "[notification]") {
    Notification n(3, 30, 300, "Repeated");
    for (int i = 0; i < 3; ++i) {
        REQUIRE_NOTHROW(n.send());
        REQUIRE_NOTHROW(n.markAsRead());
    }
}

TEST_CASE("Different notifications can be created", "[notification]") {
    Notification n1(1, 10, 100, "First");
    Notification n2(2, 20, 200, "Second");
    SUCCEED("Both created successfully");
}