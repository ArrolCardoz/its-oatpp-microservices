#ifndef ISSUE_CONTAINER_H_INCLUDED
#define ISSUE_CONTAINER_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../server/Constants.hpp"

#include "Issue.hpp"

namespace rest_its { namespace issue { namespace service {

class IssueContainer {
 public:
    /**
     * Default destructor
     * deletes all pointers in issues
     * deletes all issues
     */
    virtual ~IssueContainer();

    /**
     * Creates a new Issue object and adds it to the _issues vector
     * Generates ID to pass to Issue constructor by checking the
     * ID of the last issue in _issues and incrementing it
     * @param[in] t the title of the new Issue to be created
     */
    virtual void createIssue(std::string t);

    /**
     * Getter for the _issues vector
     * @return _issues
     */
    virtual std::vector<RealIssue*> listIssues() { return _issues; }

    /**
     * Searches _issues and returns the Issue with the given ID
     * @param[in] id the issueID we are looking for
     * @return a pointer to the Issue requested
     */
    virtual Issue* getIssue(int id);

    /**
     * Searches _issues and deletes the Issue with the given ID
     * @param[in] id the issueId we are looking for
     */
    virtual void removeIssue(int id);

    /**
     * Creates an Issue with id 0 and name "Null Issue"
     */
    virtual void setup();

    /**
     * Writes _issues to a .txt file
     * @param[in] filename The filepath for the save file
     */
    virtual void save(
    const std::string& filename = "./issueServer/src/service/data/issues.txt");

    /**
     * Reads a .txt file to create Issues
     * @param[in] filename The filepath of the file to be read
     */
    virtual void load(
    const std::string& filename = "./issueServer/src/service/data/issues.txt");

    /**
     * Used as a method to run setup() and load() on starting the server
     */
    bool getStatus() const { return initialized; }

    /**
     * Works with getStatus() to ensure setup() and load() run at start of server
     */
    void setStatus(bool b) { initialized = b; }

 private:
    std::vector<RealIssue*> _issues;
    bool initialized = false;
};

} // namespace service
} // namespace issue
} // namespace rest_its

#endif // ISSUE_CONTAINER_H_INCLUDED
