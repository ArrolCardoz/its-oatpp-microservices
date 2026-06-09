#ifndef REST_ITS_COMMENT_DTO_hpp
#define REST_ITS_COMMENT_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its {
namespace comment {
namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class CommentDto : public oatpp::DTO {
  DTO_INIT(CommentDto, DTO)

  DTO_FIELD(String, text);
};

#include OATPP_CODEGEN_END(DTO)

}  // namespace dto
}  // namespace comment
}  // namespace rest_its

#endif /* REST_ITS_COMMENT_DTO_hpp */
