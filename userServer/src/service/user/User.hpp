#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include <set>
#include <string>
#include <unordered_set>

class User {
 public:
  enum class Role { ADMIN, DEVELOPER, TESTER, VIEWER, NULL_USER };

  User(int _userID, std::string _name) {}

  virtual int getID() const = 0;
  virtual std::string getName() const = 0;
  virtual void setName(std::string n) = 0;
  virtual std::string getEmail() const = 0;
  virtual void setEmail(std::string n) = 0;
  virtual void setRole(User::Role role) = 0;
  virtual User::Role getRole() const = 0;
  virtual void incrementWorkload(int issueID) = 0;
  virtual void decrementWorkload(int issueID) = 0;
  virtual int getWorkload() const = 0;
  virtual std::unordered_set<int> getAssignedIssueID() const = 0;

  virtual std::string roleToString(User::Role role) const;

  virtual ~User() = default;
};

class RealUser : public User {
 public:
  /**
   * Default constructor
   * @param userID a unique ID to define users
   * @param name a display name for user
   * @brief sets other details to default value
   * @details default role is "viewer" and workload is 0
   */
  RealUser(int userID, std::string name)
      : User(userID, name), _userID(userID), _name(name) {
    _role = User::Role::VIEWER;
    _workload = 0;
    _email = "NotSet";
  }

  /**
   * Default destructor
   */
  virtual ~RealUser() {}

  /**
   * Getter for userID
   * @return userID
   */
  int getID() const override { return _userID; }

  /**
   * Getter for name
   * @return name
   */
  std::string getName() const override { return _name; }

  /**
   * Setter for name
   * @param n the new name
   */
  void setName(std::string n) override { _name = n; }

  /**
   * Getter for email
   * @return email
   */
  std::string getEmail() const override { return _email; }
  /**
   * setter for email
   * @param e the new email
   */
  void setEmail(std::string e) override { _email = e; }
  /**
   * setter for name
   * @param role the new role
   */
  void setRole(User::Role role) override { _role = role; }
  /**
   * Getter for role
   * @return _role
   */
  User::Role getRole() const override { return _role; }

  /**
   * helper function to increment workload and store in _assignedIssueIds
   * @param issueID the new issueID
   */
  void incrementWorkload(int issueID) override {
    _assignedIssueIds.insert(issueID);
    _workload = _assignedIssueIds.size();
  };
  /**
   * helper function to decrement workload and remove in _assignedIssueIds
   * @param issueID the existing issueID
   */
  void decrementWorkload(int issueID) override {
    _assignedIssueIds.erase(issueID);
    _workload = _assignedIssueIds.size();
  }
  /**
   * Getter for _workload
   * @return _workload
   */
  int getWorkload() const override { return _workload; }
  /**
   * Getter for _assignedIssueIds
   * @return _assignedIssueIds
   */
  std::unordered_set<int> getAssignedIssueID() const override {
    return _assignedIssueIds;
  }

 private:
  int _userID;
  std::string _name;
  std::string _email;
  Role _role;
  int _workload;
  std::unordered_set<int> _assignedIssueIds;
};

class NullUser : public User {
 public:
  /**
   * Default constructor
   */
  NullUser(int userID, std::string name) : User(userID, name) {}

  /**
   * Default destructor
   */
  virtual ~NullUser() {}

  /**
   * Overrides User:getID()
   * @return 0
   */
  int getID() const override { return 0; }

  /**
   * Overrides User:getName()
   * @return "Null User"
   */
  std::string getName() const override { return "Null User"; }

  void setName(std::string n) override {}

  /**
   * Getter for email
   * @return email
   */
  std::string getEmail() const override { return "Null Email"; }

  void setEmail(std::string e) override {}
  /**
   * Getter for user
   * @return Null user
   */
  User::Role getRole() const override { return Role::NULL_USER; }

  void setRole(User::Role role) override {}
  /**
   * helper function to increment workload and store in _assignedIssueIds
   * @param issueID the new issueID
   */
  void incrementWorkload(int issueID) override {}
  /**
   * helper function to decrement workload and remove in _assignedIssueIds
   * @param issueID the existing issueID
   */
  void decrementWorkload(int issueID) override {}
  /**
   * Getter for 0
   * @return _workload
   */
  int getWorkload() const override { return 0; }
  /**
   * Getter for _assignedIssueIds
   * @return {}
   */
  std::unordered_set<int> getAssignedIssueID() const override { return {}; };
};

#endif
