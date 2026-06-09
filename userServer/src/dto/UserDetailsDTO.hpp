#ifndef GET_USER_DETAILS_DTO_hpp
#define GET_USER_DETAILS_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace rest_its {namespace user {namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDetailsDTO : public oatpp::DTO {
    DTO_INIT(UserDetailsDTO, DTO)

    DTO_FIELD(Int32, id);
    DTO_FIELD(String, name);
    DTO_FIELD(String, email);
    DTO_FIELD(Int32, role);
    DTO_FIELD(Int32, workload);
    DTO_FIELD(List<Int32>, issues);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace user
} // namespace rest_its

#endif
