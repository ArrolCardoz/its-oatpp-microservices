#include "Runner.hpp"

#include "AppComponent.hpp"
#include "Constants.hpp"
#include "controller/CommentController.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"

namespace rest_its {
namespace comment {

Runner::Runner() {
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
                  Qualifiers::SERVICE_COMMENT);

  oatpp::web::server::api::Endpoints docEndpoints;

  // Register CommentController
  docEndpoints.append(
      router->addController(std::make_shared<controller::CommentController>())
          ->getEndpoints());

  // Register Swagger UI
  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo,
                  Qualifiers::SERVICE_COMMENT);
  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources,
                  Qualifiers::SERVICE_COMMENT);

  router->addController(oatpp::swagger::Controller::createShared(
      docEndpoints, documentInfo, resources));
}

void Runner::run(std::list<std::thread>& acceptingThreads) {
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
                  Qualifiers::SERVICE_COMMENT);

  auto connectionHandler =
      oatpp::web::server::HttpConnectionHandler::createShared(router);

  connectionHandler->addRequestInterceptor(
      std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
  connectionHandler->addResponseInterceptor(
      std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());
  connectionHandler->addResponseInterceptor(
      std::make_shared<
          rest_its::comment::controller::CustomCorsMethodsInterceptor>());

  // TCP server
  acceptingThreads.push_back(std::thread([router, connectionHandler] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    connectionProvider, Qualifiers::SERVICE_COMMENT);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("comment", "Server listening on port '%s'",
               connectionProvider->getProperty("port").getData());
    server.run();
  }));

  acceptingThreads.push_back(std::thread([router, connectionHandler] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    connectionProvider, Qualifiers::SERVICE_COMMENT_VH);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("comment", "Server listening on virtual interface '%s'",
               connectionProvider->getProperty("host").getData());
    server.run();
  }));
}

}  // namespace comment
}  // namespace rest_its
