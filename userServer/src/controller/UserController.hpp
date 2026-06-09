#ifndef UserController_hpp
#define UserController_hpp

#include <memory>

#include "../server/Constants.hpp"

#include "../dto/CreateUserDTO.hpp"
#include "../dto/UpdateUserDTO.hpp"
#include "../dto/UpdateWorkloadDTO.hpp"
#include "../dto/UserDetailsDTO.hpp"
#include "../dto/UserCreatedDTO.hpp"

#include "../service/user/UserContainer.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

namespace rest_its { namespace user { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class CustomCorsMethodsInterceptor
  : public oatpp::web::server::interceptor::ResponseInterceptor {
public:
  std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
  intercept(const std::shared_ptr<oatpp::web::server::api::ApiController::IncomingRequest>& request,
            const std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>& response) override {

    using namespace oatpp::web::protocol::http;

    // Overwrite the header set by AllowCorsGlobal
    response->putHeader(Header::CORS_METHODS,
                        "GET, POST, PUT, DELETE, OPTIONS");

    return response;
  }
};

class UserController : public oatpp::web::server::api::ApiController {
 private:
  std::shared_ptr<rest_its::user::service::UserContainer> user;

 public:
  UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
                OATPP_COMPONENT(std::shared_ptr<
                rest_its::user::service::UserContainer>,
                userContainer)
  )
    : oatpp::web::server::api::ApiController(objectMapper)
    , user(userContainer)
    {}

 public:
    ENDPOINT_INFO(createUser) {
        info->summary = "Create a new user.";
        info->addConsumes<Object<
                        rest_its::user::dto::CreateUserDTO>>(
                        "application/json");
    }

    ENDPOINT("POST", "/users", createUser,
            BODY_DTO(Object<rest_its::user::dto::CreateUserDTO>, body)) {
    OATPP_ASSERT_HTTP(body && body->name, Status::CODE_400, "Missing 'Name'.");

    if (!user->getStatus()) {
      user->load();
      user->setup();
      user->setStatus(true);
    }
    user->createUser(body->name);
    user->save();

    auto responseDTO = rest_its::user::dto::UserCreatedDTO::createShared();
    responseDTO->id = sizeof(user->listUsers()) - 1;
    responseDTO->message = "User Created.\n";

    return createDtoResponse(Status::CODE_200, responseDTO);
    }

    ENDPOINT_INFO(deleteUser) {
      info->summary = "Deletes a user by ID.";
    }

    ENDPOINT("DELETE", "/users/{id}", deleteUser, PATH(Int32, id)) {
      if (!user->getStatus()) {
        user->load();
        user->setup();
        user->setStatus(true);
      }

      user->removeUser(id);
      user->save();

      return createResponse(Status::CODE_200, "User Deleted.\n");
    }

    ENDPOINT_INFO(updateUser) {
      info->summary = "Updates user name, email, and/or role by ID";
    }

    ENDPOINT("PUT", "/users/{id}", updateUser,
            PATH(Int32, id),
            BODY_DTO(Object<rest_its::user::dto::UpdateUserDTO>, body)) {
      if (!user->getStatus()) {
        user->load();
        user->setup();
        user->setStatus(true);
      }

      auto u = user->getUser(id);

      OATPP_ASSERT_HTTP(u != nullptr, Status::CODE_404, "User not found.\n");

      if (body->name) {
        u->setName(body->name->c_str());
      }

      if (body->email) {
        u->setEmail(body->email->c_str());
      }

      if (body->role) {
        if (body->role == "ADMIN") {
          u->setRole(User::Role::ADMIN);
        } else if (body->role == "DEVELOPER") {
          u->setRole(User::Role::DEVELOPER);
        } else if (body->role == "TESTER") {
          u->setRole(User::Role::TESTER);
        } else {
          u->setRole(User::Role::VIEWER);
        }
      }

      user->save();

      return createResponse(Status::CODE_200, "User Updated.\n");
    };

    ENDPOINT_INFO(updateWorkload) {
      info->summary = "Updates user workload by ID.";
    }

    ENDPOINT("PUT", "/user/{id}/workload", updateWorkload,
            PATH(Int32, id),
            BODY_DTO(Object<rest_its::user::dto::UpdateWorkloadDTO>, body)) {
      if (!user->getStatus()) {
        user->load();
        user->setup();
        user->setStatus(true);
      }

      auto u = user->getUser(id);

      if (dynamic_cast<NullUser*>(u) != nullptr) {
        return createResponse(Status::CODE_404, "User Not Found.\n");
      }

      if (body->assign) {
        for (auto issue : *body->assign) {
          u->incrementWorkload(issue);
        }
      }

      if (body->unassign) {
        for (auto issue : *body->unassign) {
          u->decrementWorkload(issue);
        }
      }

      user->save();

      return createResponse(Status::CODE_200, "Workload Updated.\n");
    }

    ENDPOINT_INFO(getUsers) {
      info->summary = "Gets all users.";
    }

    ENDPOINT("GET", "/users", getUsers) {
      if (!user->getStatus()) {
        user->load();
        user->setup();
        user->setStatus(true);
      }

      auto list = oatpp::List<oatpp::Object<
                  rest_its::user::dto::UserDetailsDTO>>::createShared();

      for (auto u : user->listUsers()) {
        auto dto = rest_its::user::dto::UserDetailsDTO::createShared();
        dto->id = u->getID();
        dto->name = u->getName().c_str();
        dto->email = u->getEmail().c_str();
        dto->role = static_cast<v_int32>(u->getRole());
        dto->workload = u->getWorkload();

        dto->issues = oatpp::List<Int32>::createShared();
        for (auto issue : u->getAssignedIssueID()) {
          dto->issues->push_back(issue);
        }

        list->push_back(dto);
      }

      return createDtoResponse(Status::CODE_200, list);
    }
};

} // namespace controller
} // namespace user
} // namespace rest_its

#endif
