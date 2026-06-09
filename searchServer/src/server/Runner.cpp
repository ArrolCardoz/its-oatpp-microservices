#include "Runner.hpp"

#include "AppComponent.hpp"
#include "controller/SearchController.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"

namespace rest_its {
namespace search {

Runner::Runner() {
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
                  Qualifiers::SERVICE_SEARCH);

  oatpp::web::server::api::Endpoints docEndpoints;

  docEndpoints.append(
      router->addController(std::make_shared<controller::SearchController>())
          ->getEndpoints());

  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo,
                  Qualifiers::SERVICE_SEARCH);
  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources,
                  Qualifiers::SERVICE_SEARCH);

  router->addController(oatpp::swagger::Controller::createShared(
      docEndpoints, documentInfo, resources));
}

void Runner::run(std::list<std::thread>& acceptingThreads) {
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
                  Qualifiers::SERVICE_SEARCH);

  auto connectionHandler =
      oatpp::web::server::HttpConnectionHandler::createShared(router);
  
  connectionHandler->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
  connectionHandler->addResponseInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());

  acceptingThreads.push_back(std::thread([router, connectionHandler] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    connectionProvider, Qualifiers::SERVICE_SEARCH);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("search", "server is listening on port '%s'",
               connectionProvider->getProperty("port").getData());
    server.run();
  }));

  acceptingThreads.push_back(std::thread([router, connectionHandler] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    connectionProvider, Qualifiers::SERVICE_SEARCH_VH);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("search", "server is listening on virtual interface '%s'",
               connectionProvider->getProperty("host").getData());
    server.run();
  }));
}

}  // namespace search
}  // namespace rest_its
