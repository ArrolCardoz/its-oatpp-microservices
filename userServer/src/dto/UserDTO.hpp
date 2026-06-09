#ifndef USER_DTO_hpp
#define USER_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its {namespace user {namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, name);
  DTO_FIELD(String, email);
  DTO_FIELD(Int32, role);
  DTO_FIELD(List<Int32>, issues);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace user
} // namespace rest_its

#endif
