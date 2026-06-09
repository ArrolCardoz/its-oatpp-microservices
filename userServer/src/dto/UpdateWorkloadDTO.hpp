#ifndef UPDATE_USER_WORKLOAD_DTO_hpp
#define UPDATE_USER_WORKLOAD_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace rest_its {namespace user {namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class UpdateWorkloadDTO : public oatpp::DTO {
    DTO_INIT(UpdateWorkloadDTO, DTO)

    DTO_FIELD(List<oatpp::Int32>, assign);
    DTO_FIELD(List<oatpp::Int32>, unassign);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace dto
} // namespace user
} // namespace rest_its

#endif
