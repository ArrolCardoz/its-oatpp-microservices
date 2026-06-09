#ifndef geometry_comment_SwaggerComponent_hpp
#define geometry_comment_SwaggerComponent_hpp

#include "Constants.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/core/macro/component.hpp"

namespace rest_its {
namespace comment {

class SwaggerComponent {
 public:
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>,
                         swaggerDocumentInfo)(Qualifiers::SERVICE_COMMENT, [] {
    oatpp::swagger::DocumentInfo::Builder builder;

    builder.setTitle("Comment Service")
        .setDescription("REST API for managing comments")
        .setVersion("1.0")
        .setContactName("Arrol Cardoz")
        .setContactEmail("arrol.cardoz@uleth.ca")
        .addServer("http://localhost:8303", "server on localhost");

    return builder.build();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>,
                         swaggerResources)(Qualifiers::SERVICE_COMMENT, [] {
    const char* envRes = std::getenv("OATPP_SWAGGER_RES");
    if (envRes && std::strlen(envRes) > 0) {
      return oatpp::swagger::Resources::loadResources(envRes);
    }
    return oatpp::swagger::Resources::loadResources(
        "/usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res");
  }());
};

}  // namespace comment
}  // namespace rest_its

#endif
