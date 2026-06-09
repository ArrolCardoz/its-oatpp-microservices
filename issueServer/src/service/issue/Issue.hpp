#ifndef ISSUE_H_INCLUDED
#define ISSUE_H_INCLUDED

#include <set>
#include <string>
#include <unordered_set>

class Issue {
 public:
    enum class Priority { UNDEFINED, LOW, MEDIUM, HIGH };

    Issue(int _issueID, std::string _title) {}

    virtual int getID() const = 0;
    virtual std::string getTitle() const = 0;
    virtual void setTitle(std::string t) = 0;
    virtual Issue::Priority getPriority() const = 0;
    virtual void setPriority(Issue::Priority priority) = 0;
    virtual void assignUser(int userID) = 0;
    virtual int getAssignedUserID() const = 0;
    virtual std::string getStatus() const = 0;
    virtual void setStatus(std::string s) = 0;
    virtual std::unordered_set<int> getComments() const = 0;
    virtual void addComment(int commentID) = 0;
    virtual void removeComment(int commentID) = 0;

    virtual std::string priorityToString(Issue::Priority p) const;

    virtual ~Issue() = default;
};

class RealIssue : public Issue {
 public:
    /**
     * Default constructor
     * @param[in] i The ID number of the Issue being created
     * @param[in] t The Title of the Issue being created
     */
    RealIssue(int i, std::string t) : Issue(i, t), _id(i), _title(t) {
      _priority = Issue::Priority::UNDEFINED;
    }

    /**
     * Destructor
     */
    virtual ~RealIssue() {}

    /**
     * Retrieves the ID number of the Issue
     * @return The ID number of the Issue
     */
    int getID() const override { return _id; }

    /**
     * Retrieves the Title of the Issue
     * @return The Title of the Issue
     */
    std::string getTitle() const override { return _title; }

    /**
     * Sets the Title of the Issue
     * @param[in] t The new Title of the Issue
     */
    void setTitle(std::string t) override { _title = t; }

    /**
     * Retrieves the priority level of the issue
     * @return _priority
     */
    Issue::Priority getPriority() const override { return _priority; }

    /**
     * Sets the priority level of the issue
     * @param[in] id The priority level to set the issue, default UNDEFINED
     */
    void setPriority(Issue::Priority priority) override {
        _priority = priority;
    };

    /**
     * Gets the ID of the User assigned to the Issue
     * @return UserID
     */
    int getAssignedUserID() const override { return _userID; };

    /**
     * Assigns a User to the Issue
     * @param[in] userID The userID of the User
     */
    void assignUser(int userID) override { _userID = userID; };

    /**
     * Gets the status of the issue
     * @return The status string
     */
    std::string getStatus() const override { return _status; };

    /**
     * Sets the status of the Issue
     * @param[in] s The status string
     */
    void setStatus(std::string s) override { _status = s; };

    /**
     * Gets the list of Comment IDs from the Issue
     * @return The list of Comment IDs
     */
    std::unordered_set<int> getComments() const override { return _comments; };

    /**
     * Adds a Comment ID to the list of comments
     * @param[in] commentID the ID of the comment to be added
     */
    void addComment(int commentID) override { _comments.insert(commentID); };

    /**
     * Removes a Comment ID from the list of comments
     * @param[in] commentID the ID of the Comment to be removed
     */
    void removeComment(int commentID) override { _comments.erase(commentID); };

 private:
    int _id;
    std::string _title;
    Issue::Priority _priority = Issue::Priority::UNDEFINED;
    int _userID = 0;
    std::string _status;
    std::unordered_set<int> _comments;
};

class NullIssue : public Issue {
 public:
    /**
     * Default constructor
     */
    NullIssue(int _issueID, std::string _title) : Issue(_issueID, _title) {}

    /**
     * Default destructor
     */
    virtual ~NullIssue() {}

    /**
     * Overrides Issue::getID()
     */
    int getID() const override { return 0; }

    /**
     * Overrides Issue::getTitle()
     */
    std::string getTitle() const override { return "Null Issue"; }

    void setTitle(std::string t) override {}

    /**
     * Retrieves the priority level of the issue
     */
    Issue::Priority getPriority() const override { return Priority::UNDEFINED; }

    void setPriority(Issue::Priority priority) override {}

    void assignUser(int userID) override {}

    int getAssignedUserID() const override { return 0; }

    std::string getStatus() const override { return 0; }

    void setStatus(std::string s) override {}

    std::unordered_set<int> getComments() const override { return {}; }

    void addComment(int commentID) override {}

    void removeComment(int commentID) override {}
};

#endif // ISSUE_H_INCLUDED
