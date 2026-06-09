#ifndef ISSUE_DTO_hpp
#define ISSUE_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

// #include "CommentStore.hpp"

namespace rest_its { namespace issue { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class IssueDTO : public oatpp::DTO {
  DTO_INIT(IssueDTO, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, title);
  DTO_FIELD(Int32, user);
  DTO_FIELD(String, status);
  DTO_FIELD(Int32, priority);
  DTO_FIELD(List<Int32>, comments);
};

class CreateIssueDTO : public oatpp::DTO {
  DTO_INIT(CreateIssueDTO, DTO)

  DTO_FIELD(String, title);
};

// UpdateIssueDTO
class UpdateIssueDTO : public oatpp::DTO {
  DTO_INIT(UpdateIssueDTO, DTO)

  DTO_FIELD(String, title);
  DTO_FIELD(Int32, user);
  DTO_FIELD(String, status);
  DTO_FIELD(Int32, priority);
};

class UpdateCommentsDTO : public oatpp::DTO {
  DTO_INIT(UpdateCommentsDTO, DTO)

  DTO_FIELD(List<Int32>, addComment);
  DTO_FIELD(List<Int32>, removeComment);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace issue
} // namespace rest_its

#endif /* ISSUE_DTO_hpp */
