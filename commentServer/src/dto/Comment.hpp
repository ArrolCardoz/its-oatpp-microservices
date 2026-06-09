#ifndef COMMENT_MODEL_hpp
#define COMMENT_MODEL_hpp

#include <string>
// this is a private dto
struct Comment {
  int id;
  int issueId;
  std::string text;
};

#endif  // COMMENT_MODEL_hpp
