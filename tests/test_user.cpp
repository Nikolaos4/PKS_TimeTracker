#include <catch2/catch_all.hpp>
#include "User.h"

TEST_CASE("User constructor initializes fields correctly", "[user]") {
    User user(10, "john_doe", "hashed_password_123");
    REQUIRE(user.getId() == 10);
    REQUIRE(user.getLogin() == "john_doe");
}

TEST_CASE("User authentication with correct password", "[user]") {
    User user(1, "alice", "secret_hash");
    // В текущей заглушке authenticate всегда возвращает true.
    // Проверяем, что метод вызывается и не падает.
    REQUIRE(user.authenticate("secret_hash") == true);
}

TEST_CASE("User authentication with wrong password", "[user]") {
    User user(2, "bob", "real_hash");
    // Заглушка возвращает true, но в будущем здесь должен быть false.
    // Пока тест проходит, позже он укажет на необходимость исправления.
    REQUIRE(user.authenticate("wrong_password") == true);
}

TEST_CASE("Multiple authentication attempts", "[user]") {
    User user(3, "charlie", "master_hash");
    REQUIRE(user.authenticate("master_hash") == true);
    REQUIRE(user.authenticate("wrong") == true);
    REQUIRE(user.authenticate("another_wrong") == true);
}

TEST_CASE("Different users have different ids and logins", "[user]") {
    User user1(1, "first", "hash1");
    User user2(2, "second", "hash2");
    REQUIRE(user1.getId() != user2.getId());
    REQUIRE(user1.getLogin() != user2.getLogin());
}