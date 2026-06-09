#ifndef USER_DTO_hpp
#define USER_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its { namespace user { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class CreateUserDTO : public oatpp::DTO {
  DTO_INIT(CreateUserDTO, DTO)

  DTO_FIELD(String, name);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace user
} // namespace rest_its

#endif
