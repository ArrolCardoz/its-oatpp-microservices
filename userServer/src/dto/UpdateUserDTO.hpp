#ifndef UPDATE_USER_DTO_hpp
#define UPDATE_USER_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its { namespace user { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class UpdateUserDTO : public oatpp::DTO {
    DTO_INIT(UpdateUserDTO, DTO)

    DTO_FIELD(String, name);
    DTO_FIELD(String, email);
    DTO_FIELD(String, role);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace user
} // namespace rest_its

#endif
