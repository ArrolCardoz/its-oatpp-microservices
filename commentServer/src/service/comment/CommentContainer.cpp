#include "CommentContainer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace rest_its {
namespace comment {
namespace service {

CommentContainer::~CommentContainer() {
  _commentsByIssue.clear();
  _nextCommentIdByIssue.clear();
}

Comment CommentContainer::addComment(int issueId, const std::string& text) {
  int nextId = _nextCommentIdByIssue[issueId] + 1;
  _nextCommentIdByIssue[issueId] = nextId;

  Comment comment;
  comment.id = nextId;
  comment.issueId = issueId;
  comment.text = text;

  _commentsByIssue[issueId].push_back(comment);

  save("./commentServer/comments.json");
  // save("/comments.json");

  attachCommentToIssue(issueId, nextId);

  return comment;
}

std::vector<Comment> CommentContainer::getAllComments() const {
  std::vector<Comment> result;
  for (const auto& pair : _commentsByIssue) {
    const auto& vec = pair.second;
    result.insert(result.end(), vec.begin(), vec.end());
  }
  return result;
}

std::vector<Comment> CommentContainer::getComments(int issueId) {
  if (_commentsByIssue.find(issueId) != _commentsByIssue.end())
    return _commentsByIssue[issueId];
  return {};
}

Comment CommentContainer::getCommentById(int issueId, int commentId) {
  auto it = _commentsByIssue.find(issueId);
  if (it != _commentsByIssue.end()) {
    for (auto& c : it->second) {
      if (c.id == commentId) return c;
    }
  }
  Comment c;
  c.id = -1;
  return c;
}

bool CommentContainer::updateComment(int issueId, int commentId,
                                     const std::string& newText) {
  auto it = _commentsByIssue.find(issueId);
  if (it != _commentsByIssue.end()) {
    for (auto& c : it->second) {
      if (c.id == commentId) {
        c.text = newText;
        save("./commentServer/comments.json");
        // save("/comments.json");

        return true;
      }
    }
  }
  return false;
}

bool CommentContainer::deleteComment(int issueId, int commentId) {
  auto it = _commentsByIssue.find(issueId);
  if (it != _commentsByIssue.end()) {
    auto& vec = it->second;
    for (size_t i = 0; i < vec.size(); i++) {
      if (vec[i].id == commentId) {
        vec.erase(vec.begin() + i);
        save("./commentServer/comments.json");
        // save("/comments.json");

        detachCommentFromIssue(issueId, commentId);

        return true;
      }
    }
  }
  return false;
}

oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>
CommentContainer::loadIssues() {
  std::ifstream file("./issueServer/src/service/data/issues.txt");
  if (!file.is_open()) {
    return nullptr;
  }

  std::stringstream ss;
  ss << file.rdbuf();
  file.close();

  auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  mapper->getSerializer()->getConfig()->useBeautifier = true;

  return mapper->readFromString<
      oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>>(ss.str());
}

void CommentContainer::saveIssues(
    const oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>& issues) {
  auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  mapper->getSerializer()->getConfig()->useBeautifier = true;

  oatpp::String json = mapper->writeToString(issues);

  std::ofstream file("./issueServer/src/service/data/issues.txt",
                     std::ios::out | std::ios::trunc);

  if (!file.is_open()) return;

  file << json->c_str();
  file.close();
}

bool CommentContainer::issueExists(int issueId) {
  auto issues = loadIssues();
  if (!issues) return false;

  for (auto& issue : *issues) {
    if (issue && issue->id == issueId) return true;
  }

  return false;
}

void CommentContainer::attachCommentToIssue(int issueId, int commentId) {
  auto issues = loadIssues();
  if (!issues) return;

  for (auto& issue : *issues) {
    if (issue->id == issueId) {
      if (!issue->comments) {
        issue->comments = oatpp::List<oatpp::Int32>::createShared();
      }
      issue->comments->push_back(commentId);
      break;
    }
  }

  saveIssues(issues);
}

void CommentContainer::detachCommentFromIssue(int issueId, int commentId) {
  auto issues = loadIssues();
  if (!issues) return;

  for (auto& issue : *issues) {
    if (issue->id == issueId) {
      if (issue->comments) {
        auto list = issue->comments;

        for (auto it = list->begin(); it != list->end(); ++it) {
          if (*it == commentId) {
            list->erase(it);
            break;
          }
        }
      }

      break;
    }
  }

  saveIssues(issues);
}

void CommentContainer::load(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) return;

  std::stringstream ss;
  ss << file.rdbuf();
  file.close();

  auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  mapper->getSerializer()->getConfig()->useBeautifier = true;

  auto dtoList =
      mapper->readFromString<oatpp::List<oatpp::Object<dto::CommentOutputDto>>>(
          ss.str());

  if (!dtoList) return;

  for (const auto& dto : *dtoList) {
    if (!dto) continue;

    Comment c;
    c.id = dto->id;
    c.issueId = dto->issueId;
    c.text = dto->text;

    _commentsByIssue[c.issueId].push_back(c);

    _nextCommentIdByIssue[c.issueId] =
        std::max(_nextCommentIdByIssue[c.issueId], c.id);
  }
}

void CommentContainer::save(const std::string& filename) {
  auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
  mapper->getSerializer()->getConfig()->useBeautifier = true;

  auto dtoList =
      oatpp::List<oatpp::Object<dto::CommentOutputDto>>::createShared();

  for (auto& [issueId, vec] : _commentsByIssue) {
    for (auto& c : vec) {
      auto dto = dto::CommentOutputDto::createShared();
      dto->id = c.id;
      dto->issueId = c.issueId;
      dto->text = c.text;
      dtoList->push_back(dto);
    }
  }

  oatpp::String json = mapper->writeToString(dtoList);

  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) return;

  file << json->c_str();
  file.close();
}

}  // namespace service
}  // namespace comment
}  // namespace rest_its
