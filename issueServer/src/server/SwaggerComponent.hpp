
#ifndef its_issue_SwaggerComponent_hpp
#define its_issue_SwaggerComponent_hpp

#include <memory>

#include "Constants.hpp"

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

namespace rest_its { namespace issue {

class SwaggerComponent {
 public:
  /**
   *  General API docs info
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>,
    swaggerDocumentInfo)(Qualifiers::SERVICE_ISSUE, [] {
    oatpp::swagger::DocumentInfo::Builder builder;

    // Port is subject to change
    builder
      .setTitle("Issue Service")
      .setVersion("1.0")
      .addServer("http://localhost:8301", "server on localhost");

    return builder.build();
  }());


  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>,
    swaggerResources)(Qualifiers::SERVICE_ISSUE, [] {
    // Make sure to specify correct full path to oatpp-swagger/res folder !!!
    return oatpp::swagger::Resources::streamResources("/usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res");
  }());
};

} // namespace issue
} // namespace rest_its

#endif /* its_issue_SwaggerComponent_hpp */
