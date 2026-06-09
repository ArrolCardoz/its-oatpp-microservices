#ifndef USER_CONTAINER_H_INCLUDED
#define USER_CONTAINER_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "User.hpp"

namespace rest_its { namespace user { namespace service {

class UserContainer {
 public:
  /**
   * Default destructor
   * deletes all pointers in users
   * deletes users
   */
  virtual ~UserContainer();

  /**
   * Creates a new user object and adds it to the users vector
   * Generates ID to pass to user constructor by checking the
   * ID of the last user in users and incrementing it
   * @param n the name of the new user you want to create
   */
  virtual void createUser(std::string n);

  /**
   * Getter for our users vector
   * @return users
   */
  virtual std::vector<RealUser*> listUsers() { return users; }

  /**
   * Searches users and returns the user with the given id
   * @param id the userID we are looking for
   * @return a pointer to the User requested
   */
  virtual User* getUser(int id);

  /**
   * Searches users and deletes the user with the given id
   *
   * @param id the userID we are looking for
   */
  virtual void removeUser(int id);

  /**
   * creates unassigned user with id 0 and name "Unassigned"
   */
  virtual void setup();

  /**
   * @param filename the filepath
   * Writes users to a .txt file
   */
  virtual void save(const std::string& filename =
    "./userServer/src/service/data/users.txt");

  /**
   * @param filename the filepath
   * Reads a .txt file to create users
   */
  virtual void load(const std::string& filename =
    "./userServer/src/service/data/users.txt");

  /**
   * Used as a method to run setup() and load() on starting the server
   */
  bool getStatus() const { return initialized; }

  /**
   * Works with getStatus() to ensure setup() and load() run at start of server
   */
  void setStatus(bool b) { initialized = b; }


 private:
  std::vector<RealUser*> users;
  bool initialized = false;
};

} // namespace service
} // namespace user
} // namespace rest_its

#endif
