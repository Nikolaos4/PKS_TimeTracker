#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    int id;
    std::string login;
    std::string passwordHash;
public:
    User(int id, const std::string& login, const std::string& hash);
    bool authenticate(const std::string& password);
    int getId() const;
    std::string getLogin() const;
};

#endif