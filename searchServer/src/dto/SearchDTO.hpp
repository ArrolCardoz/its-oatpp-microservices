#ifndef SearchDto_hpp
#define SearchDto_hpp

#include "IssueDTO.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its { namespace search { namespace dto {

/* Begin DTO code generation */
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Data Transfer Object for Searchs
 */
class SearchDto : public oatpp::DTO {
  DTO_INIT(SearchDto, DTO)

  DTO_FIELD(List<Object<rest_its::search::dto::IssueDto>>, issues);
};

#include OATPP_CODEGEN_END(DTO)
/* End DTO */

}}} // end namespace defs

#endif
