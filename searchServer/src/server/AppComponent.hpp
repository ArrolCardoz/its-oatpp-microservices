#ifndef rest_its_search_AppComponent_hpp
#define rest_its_search_AppComponent_hpp

#include "SwaggerComponent.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/ConnectionPool.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

namespace rest_its {
namespace search {

struct HostPort {
  oatpp::String host;
  v_uint16 port;
};

class AppComponent {
 private:
  HostPort m_search;

 public:
  AppComponent(const HostPort& search) : m_search(search) {}

  SwaggerComponent swaggerComponent;

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>,
                         virtualInterface)(Qualifiers::SERVICE_SEARCH, [] {
    return oatpp::network::virtual_::Interface::obtainShared(
        "search.virtualhost");
  }());

  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      serverConnectionProvider)(Qualifiers::SERVICE_SEARCH, [this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {m_search.host, m_search.port});
  }());

  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      virtualConnectionProvider)(Qualifiers::SERVICE_SEARCH_VH, [] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>,
                    interface, Qualifiers::SERVICE_SEARCH);
    return oatpp::network::virtual_::server::ConnectionProvider::createShared(
        interface);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                         httpRouter)(Qualifiers::SERVICE_SEARCH, [] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                         objectMapper)(Qualifiers::SERVICE_SEARCH, [] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());
};

}  // namespace search
}  // namespace rest_its

#endif  // rest_its_search_AppComponent_hpp
