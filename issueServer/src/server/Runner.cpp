#include <memory>
#include "Runner.hpp"

#include "AppComponent.hpp"

#include "controller/IssueController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"

namespace rest_its { namespace issue {

Runner::Runner() {
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
    Qualifiers::SERVICE_ISSUE);

  oatpp::web::server::api::Endpoints docEndpoints;

  /* Add CircleController */
  docEndpoints.append(router->addController(
    std::make_shared<
      rest_its::issue::controller::IssueController>())->getEndpoints());

  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo,
    Qualifiers::SERVICE_ISSUE);
  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources,
    Qualifiers::SERVICE_ISSUE);

  router->addController(oatpp::swagger::Controller::createShared(
    docEndpoints, documentInfo, resources));
}

void Runner::run(std::list<std::thread>& acceptingThreads) {
  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
    Qualifiers::SERVICE_ISSUE);

  /* Create connection handler */
  auto connectionHandler =
    oatpp::web::server::HttpConnectionHandler::createShared(router);
    
  connectionHandler->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
  connectionHandler->addResponseInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());
  connectionHandler->addResponseInterceptor(std::make_shared<rest_its::issue::controller::CustomCorsMethodsInterceptor>());

  acceptingThreads.push_back(std::thread([router, connectionHandler]{
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      connectionProvider, Qualifiers::SERVICE_ISSUE);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("issue-service", "server is listening on port '%s'",
      connectionProvider->getProperty("port").getData());
    server.run();
  }));

  acceptingThreads.push_back(std::thread([router, connectionHandler]{
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      connectionProvider, Qualifiers::SERVICE_ISSUE_VH);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("issue-service", "server is listening on virtual interface '%s'",
      connectionProvider->getProperty("host").getData());
    server.run();
  }));
}

} // namespace issue
} // namespace rest_its
