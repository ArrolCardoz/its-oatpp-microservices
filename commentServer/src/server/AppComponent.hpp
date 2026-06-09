#ifndef rest_its_comment_AppComponent_hpp
#define rest_its_comment_AppComponent_hpp

#include "../service/comment/CommentContainer.hpp"
#include "SwaggerComponent.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/ConnectionPool.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

namespace rest_its {
namespace comment {

struct HostPort {
  oatpp::String host;
  v_uint16 port;
};

class AppComponent {
 private:
  HostPort m_comment;
  HostPort m_virtualHost;

 public:
 public:
  AppComponent(const HostPort& comment, const HostPort& virtualHost)
      : m_comment(comment), m_virtualHost(virtualHost) {}

  AppComponent(const HostPort& comment)
      : m_comment(comment), m_virtualHost({"comment.virtualhost", 0}) {}

  /** Swagger */
  SwaggerComponent swaggerComponent;

  /**
   * Virtual host interface
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>,
                         virtualInterface)(Qualifiers::SERVICE_COMMENT, [this] {
    return oatpp::network::virtual_::Interface::obtainShared(
        m_virtualHost.host);
  }());

  /**
   * Real (external) server connection
   */
  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      serverConnectionProvider)(Qualifiers::SERVICE_COMMENT, [this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {m_comment.host, m_comment.port});
  }());

  /**
   * Virtual server connection
   */
  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      virtualConnectionProvider)(Qualifiers::SERVICE_COMMENT_VH, [] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>,
                    interface, Qualifiers::SERVICE_COMMENT);
    return oatpp::network::virtual_::server::ConnectionProvider::createShared(
        interface);
  }());

  /**
   * Router
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                         httpRouter)(Qualifiers::SERVICE_COMMENT, [] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  /**
   * ObjectMapper
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                         objectMapper)(Qualifiers::SERVICE_COMMENT, [] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  /**
   * CommentContainer
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<service::CommentContainer>,
                         commentContainer)(Qualifiers::SERVICE_COMMENT, [] {
    auto container = std::make_shared<service::CommentContainer>();
    container->load("./commentServer/comments.json");
    // container->load("comments.json");

    return container;
  }());
};

}  // namespace comment
}  // namespace rest_its

#endif
