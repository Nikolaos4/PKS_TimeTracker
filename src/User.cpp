#include "User.h"
#include <iostream>

User::User(int id, const std::string& login, const std::string& hash)
    : id(id), login(login), passwordHash(hash) {}

bool User::authenticate(const std::string& password) {
    std::cout << "Аутентификация пользователя " << login << std::endl;
    return true;
}

int User::getId() const { return id; }
std::string User::getLogin() const { return login; }