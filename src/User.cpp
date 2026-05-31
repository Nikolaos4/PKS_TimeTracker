#include "User.h"
#include <iostream>

User::User(int id, const std::string& login, const std::string& hash)
    : id(id), login(login), passwordHash(hash) {}

int User::getId() const { return id; }
std::string User::getLogin() const { return login; }

// Метод authenticate удалён, аутентификация происходит через DatabaseFacade