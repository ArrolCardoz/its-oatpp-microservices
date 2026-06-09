#include "User.hpp"

#include <string>

std::string User::roleToString(User::Role role) const {
  switch (role) {
    case User::Role::ADMIN:
      return "Admin";
    case User::Role::DEVELOPER:
      return "Developer";
    case User::Role::TESTER:
      return "Tester";
    case User::Role::VIEWER:
      return "Viewer";
    case User::Role::NULL_USER:
      return "Null User";
    default:
      return "Unknown";
  }
}
