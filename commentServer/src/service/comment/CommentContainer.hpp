#ifndef COMMENT_CONTAINER_HPP_INCLUDED
#define COMMENT_CONTAINER_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "../../dto/Comment.hpp"
#include "../../dto/CommentDTO.hpp"
#include "../../dto/CommentOutputDTO.hpp"
#include "../../dto/IssueDTO.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace rest_its {
namespace comment {
namespace service {

/**
 * @brief Container to manage comments in memory and persist them to JSON file
 */
class CommentContainer {
 public:
  CommentContainer() = default;
  ~CommentContainer();

  /**
   * @brief Add a new comment
   * @param issueId The issue ID this comment belongs to
   * @param text The comment text
   * @return The created Comment object
   */
  Comment addComment(int issueId, const std::string& text);

  /**
   * @brief Get all comments (across all issues)
   */
  std::vector<Comment> getAllComments() const;

  /**
   * @brief Get list of comments for an issue
   * @param issueId The issue ID
   * @return Vector of Comment objects (empty if none)
   */
  std::vector<Comment> getComments(int issueId);

  /**
   * @brief Get a single comment by issue ID and comment ID
   * @param issueId The issue ID
   * @param commentId The comment ID
   * @return Comment object if found; otherwise returns a Comment with id = -1.
   */
  Comment getCommentById(int issueId, int commentId);

  /**
   * @brief Update a comment text
   * @param issueId The issue ID
   * @param commentId The comment ID
   * @param newText The new text
   * @return true if updated, false if not found
   */
  bool updateComment(int issueId, int commentId, const std::string& newText);

  /**
   * @brief Checks in issue Server if issue exists
   * @param issueId The issue ID
   * @return  true if issue exists, false if not found
   */
  bool issueExists(int issueId);
  /**
   * @brief Load all issues from issueServer.
   * @return The list of all issues
   */
  oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>> loadIssues();

  /**
   * @brief Save updated issues back to issues.txt.
   * @param issueId The issue ID
   * @param commentId The comment ID
   */
  void saveIssues(
      const oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>&
          issues);

  /**
   * @brief Attach a new commentId to issue’s comments[] list.
   * @param issueId The issue ID
   * @param commentId The comment ID
   */
  void attachCommentToIssue(int issueId, int commentId);

  /**
   * @brief Remove a commentId from issue’s comments[] list.
   * @param issueId The issue ID
   * @param commentId The comment ID
   */
  void detachCommentFromIssue(int issueId, int commentId);
  /**
   * @brief Delete a comment
   * @param issueId The issue ID
   * @param commentId The comment ID
   * @return true if deleted, false if not found
   */
  bool deleteComment(int issueId, int commentId);

  /**
   * @brief Load comments from JSON file
   * @param filename The file to load from comments.jason
   */
  void load(const std::string& filename);

  /**
   * @brief Save comments to JSON file
   * @param filename The file to save to comments.jason
   */
  void save(const std::string& filename);

 private:
  std::unordered_map<int, std::vector<Comment>> _commentsByIssue;
  std::unordered_map<int, int> _nextCommentIdByIssue;
};

}  // namespace service
}  // namespace comment
}  // namespace rest_its

#endif  // COMMENT_CONTAINER_HPP_INCLUDED
