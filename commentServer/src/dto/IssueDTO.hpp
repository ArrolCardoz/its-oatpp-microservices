#ifndef IssueDto_hpp
#define IssueDto_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its {
namespace search {
namespace dto {

/* Begin DTO code generation */
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Data Transfer Object for issues
 */
class IssueDto : public oatpp::DTO {
  DTO_INIT(IssueDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, title);
  DTO_FIELD(Int32, user);
  DTO_FIELD(String, status);
  DTO_FIELD(Int32, priority);
  DTO_FIELD(List<Int32>, comments);
};

#include OATPP_CODEGEN_END(DTO)
/* End DTO */

}  // namespace dto
}  // namespace search
}  // namespace rest_its

#endif
