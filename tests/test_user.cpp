#include <catch2/catch_all.hpp>
#include "DatabaseFacade.h"
#include "User.h"

TEST_CASE("User: constructor initializes fields correctly", "[user]") {
    User user(10, "john", "hash123");
    REQUIRE(user.getId() == 10);
    REQUIRE(user.getLogin() == "john");
}

TEST_CASE("User: different users have different ids", "[user]") {
    User u1(1, "a", "h1");
    User u2(2, "b", "h2");
    REQUIRE(u1.getId() != u2.getId());
    REQUIRE(u1.getLogin() != u2.getLogin());
}

// Следующие тесты используют реальную БД через фасад
TEST_CASE("User: registration with unique login succeeds", "[user][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "test_reg_" + std::to_string(rand());
    REQUIRE(db.registerUser(login, "pass") == true);
}

TEST_CASE("User: registration with duplicate login fails", "[user][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "test_dup_" + std::to_string(rand());
    REQUIRE(db.registerUser(login, "pass") == true);
    REQUIRE(db.registerUser(login, "pass2") == false);
}

TEST_CASE("User: authentication with correct password", "[user][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "test_auth_" + std::to_string(rand());
    std::string pass = "secret";
    REQUIRE(db.registerUser(login, pass) == true);
    REQUIRE(db.authenticateUser(login, pass) == true);
}

TEST_CASE("User: authentication with wrong password fails", "[user][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "test_wrong_" + std::to_string(rand());
    std::string pass = "correct";
    REQUIRE(db.registerUser(login, pass) == true);
    REQUIRE(db.authenticateUser(login, "wrong") == false);
}

TEST_CASE("User: getUserIdByLogin returns correct id", "[user][db]") {
    DatabaseFacade& db = DatabaseFacade::getInstance();
    std::string login = "test_id_" + std::to_string(rand());
    REQUIRE(db.registerUser(login, "pass") == true);
    int id = db.getUserIdByLogin(login);
    REQUIRE(id > 0);
}