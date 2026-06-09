#ifndef rest_its_search_SwaggerComponent_hpp
#define rest_its_search_SwaggerComponent_hpp

#include "Constants.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

namespace rest_its {
namespace search {

class SwaggerComponent {
 public:
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>,
                         documentInfo)(Qualifiers::SERVICE_SEARCH, [] {
    oatpp::swagger::DocumentInfo::Builder builder;
    builder.setTitle("Search Service")
        .setVersion("1.0")
        .addServer("http://localhost:8304", "search server");
    return builder.build();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>,
                         resources)(Qualifiers::SERVICE_SEARCH, [] {
    const char* envRes = std::getenv("OATPP_SWAGGER_RES");
    if (envRes && std::strlen(envRes) > 0) {
      return oatpp::swagger::Resources::loadResources(envRes);
    }
    // Fallback to packaged resources for common install (oatpp-swagger 1.3.0)
    return oatpp::swagger::Resources::loadResources(
        "/usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res");
  }());
};

}  // namespace search
}  // namespace rest_its

#endif  // rest_its_search_SwaggerComponent_hpp
