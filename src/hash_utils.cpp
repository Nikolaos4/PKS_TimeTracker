#include "hash_utils.h"
#include <functional>
#include <sstream>
#include <iomanip>

std::string hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    size_t hash = hasher(password);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}