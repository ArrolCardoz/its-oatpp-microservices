#ifndef its_IssueController_hpp
#define its_IssueController_hpp

#include <memory>
#include <typeinfo>

#include "../server/Constants.hpp"
#include "../dto/issueDTOs.hpp"
#include "../service/issue/IssueContainer.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

namespace rest_its { namespace issue { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController) // begin codegen

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

class IssueController : public oatpp::web::server::api::ApiController {
 private:
    std::shared_ptr<rest_its::issue::service::IssueContainer> issue;

 public:
  IssueController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
  OATPP_COMPONENT(std::shared_ptr<rest_its::issue::service::IssueContainer>,
    issueContainer)) :
    oatpp::web::server::api::ApiController(objectMapper),
    issue(issueContainer) {}

 public:
    ENDPOINT_INFO(createIssue) {
        info->summary = "Create a new issue.";
        info->addConsumes<Object<
          rest_its::issue::dto::CreateIssueDTO>>("application/json");
    }
    ENDPOINT("POST", "/issues", createIssue, BODY_DTO(
      Object<rest_its::issue::dto::CreateIssueDTO>, body)) {
      OATPP_ASSERT_HTTP(body && body->title, Status::CODE_400,
        "Missing 'title'.");

      if (!issue->getStatus()) {
        issue->load();
        issue->setup();
        issue->setStatus(true);
      }
      issue->createIssue(body->title);
      issue->save();

      return createResponse(Status::CODE_200, "Issue Created.\n");
    }

    ENDPOINT_INFO(deleteIssue) {
      info->summary = "Delete an issue by ID.";
      info->pathParams["id"].description = "ID of the issue to be deleted.";
    }

    ENDPOINT("DELETE", "/issues/{id}", deleteIssue, PATH(Int32, id)) {
      if (!issue->getStatus()) {
        issue->load();
        issue->setup();
        issue->setStatus(true);
      } else {
        issue->load();
      }

      if (id == 0) {
        createResponse(Status::CODE_403, "Operation Forbidden");
      }

      issue->removeIssue(id);
      issue->save();

      return createResponse(Status::CODE_200, "Issue Deleted\n");
    }

    ENDPOINT_INFO(updateIssue) {
      info->summary = "Updates title, user, status, or priority of the issue.";
      info->addResponse<oatpp::Object<dto::IssueDTO>>(
        Status::CODE_200, "application/json");
      info->pathParams["id"].description = "ID of the issue to be updated.";
    }
    ENDPOINT("PUT", "/issues/{id}", updateIssue, PATH(Int32, id),
      BODY_DTO(Object<rest_its::issue::dto::UpdateIssueDTO>, body)) {
      if (!issue->getStatus()) {
        issue->load();
        issue->setup();
        issue->setStatus(true);
      } else {
        issue->load();
      }

      auto i = issue->getIssue(id);

      OATPP_ASSERT_HTTP(i != nullptr, Status::CODE_404, "Issue not found.\n");

      if (body->title) {
        i->setTitle(body->title->c_str());
      }

      if (body->user) {

        i->assignUser(body->user);
      }

      if (body->status) {
        i->setStatus(body->status->c_str());
      }

      if (body->priority) {
        if (body->priority == 1) i->setPriority(Issue::Priority::LOW);
        else if (body->priority == 2) i->setPriority(Issue::Priority::MEDIUM);
        else if (body->priority == 3) i->setPriority(Issue::Priority::HIGH);
        else
          i->setPriority(Issue::Priority::UNDEFINED);
      }

      issue->save();

      return createResponse(Status::CODE_200, "Issue Updated.\n");
    }

    ENDPOINT_INFO(getIssue) {
      info->summary = "Get issue by id.";
      info->addResponse<oatpp::Object<dto::IssueDTO>>(
        Status::CODE_200, "application/json");
      info->pathParams["id"].description = "ID of the issue requested.";
    }
    ENDPOINT("GET", "/issues/{id}", getIssue, PATH(Int32, id)) {
      if (!issue->getStatus()) {
        issue->load();
        issue->setup();
        issue->setStatus(true);
      } else {
        issue->load();
      }

      auto i = issue->getIssue(id);

      if (dynamic_cast<NullIssue*>(i) != nullptr) {
        return createResponse(Status::CODE_404, "Issue Not Found.\n");
      }

      auto dto = rest_its::issue::dto::IssueDTO::createShared();

      dto->id = i->getID();
      dto->title = i->getTitle().c_str();
      dto->priority = static_cast<v_int32>(i->getPriority());
      dto->user = i->getAssignedUserID();
      dto->status = i->getStatus().c_str();

      dto->comments = oatpp::List<Int32>::createShared();
      for (auto comment : i->getComments()) {
        dto->comments->push_back(comment);
      }

      return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(getIssues) {
      info->summary = "Lists all issues.";
      info->addResponse<oatpp::Object<dto::IssueDTO>>(
        Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/issues", getIssues) {
      if (!issue->getStatus()) {
        issue->load();
        issue->setup();
        issue->setStatus(true);
      } else {
        issue->load();
      }

      auto list = oatpp::List<oatpp::Object<
        rest_its::issue::dto::IssueDTO>>::createShared();

      for (auto i : issue->listIssues()) {
        auto dto = rest_its::issue::dto::IssueDTO::createShared();
        dto->id = i->getID();
        dto->title = i->getTitle();
        dto->priority = static_cast<v_int32>(i->getPriority());
        dto->user = i->getAssignedUserID();
        dto->status = i->getStatus();

        dto->comments = oatpp::List<Int32>::createShared();
        for (auto comment : i->getComments()) {
          dto->comments->push_back(comment);
        }

        list->push_back(dto);
      }

      return createDtoResponse(Status::CODE_200, list);
    }

};

#include OATPP_CODEGEN_END(ApiController) // end codegen

} // namespace controller
} // namespace issue
} // namespace rest_its

#endif // its_IssueController_hpp
