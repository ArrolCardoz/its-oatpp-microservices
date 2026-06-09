#ifndef USER_CREATED_DTO_hpp
#define USER_CREATED_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its { namespace user { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class UserCreatedDTO : public oatpp::DTO {
  DTO_INIT(UserCreatedDTO, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace user
} // namespace rest_its

#endif
