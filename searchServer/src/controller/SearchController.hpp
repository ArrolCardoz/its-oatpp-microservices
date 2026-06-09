#ifndef rest_its_search_SearchController_hpp
#define rest_its_search_SearchController_hpp

#include "../dto/SearchDTO.hpp"
#include "../dto/IssueDTO.hpp"
#include "../service/SearchService.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

namespace rest_its {
namespace search {
namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class SearchController : public oatpp::web::server::api::ApiController {
 public:
  SearchController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                    objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  // Search issues by IssueID
  ENDPOINT_INFO(searchIssuesByID) {
    info->summary = "Search issues by Issue ID";
    info->pathParams["id"].description = "ID of the issue to search for";
    info->addResponse<oatpp::Object<rest_its::search::dto::SearchDto>>(
        Status::CODE_200, "application/json");
    info->addResponse<oatpp::String>(Status::CODE_404, "Not found");
  }

  ENDPOINT("GET", "/search/issueID/{id}", searchIssuesByID, PATH(Int32, id)) {
    auto svc = rest_its::search::service::SearchService();
    auto dto = svc.searchByIssueID(id);
    if (!dto) return createResponse(Status::CODE_404, "Not found");
    return createDtoResponse(Status::CODE_200, dto);
  }

  // Search issues by UserID
  ENDPOINT_INFO(searchIssuesByUser) {
    info->summary = "Search issues by User ID";
    info->pathParams["id"].description = "ID of the user to search for their issues";
    info->addResponse<oatpp::Object<rest_its::search::dto::SearchDto>>(
        Status::CODE_200, "application/json");
    info->addResponse<oatpp::String>(Status::CODE_404, "Not found");
  }

  ENDPOINT("GET", "/search/userID/{id}", searchIssuesByUser, PATH(Int32, id)) {
    auto svc = rest_its::search::service::SearchService();
    auto dto = svc.searchByUserID(id);
    if (!dto) return createResponse(Status::CODE_404, "Not found");
    return createDtoResponse(Status::CODE_200, dto);
  }

  // Search issues by Status
  ENDPOINT_INFO(searchIssuesByStatus) {
    info->summary = "Search issues by Issue Status";
    info->pathParams["status"].description = "Status of the issues to search for";
    info->addResponse<oatpp::Object<rest_its::search::dto::SearchDto>>(
        Status::CODE_200, "application/json");
    info->addResponse<oatpp::String>(Status::CODE_404, "Not found");
  }

  ENDPOINT("GET", "/search/issueStatus/{status}", searchIssuesByStatus, PATH(String, status)) {
    auto svc = rest_its::search::service::SearchService();
    auto dto = svc.searchByStatus(status);
    if (!dto) return createResponse(Status::CODE_404, "Not found");
    return createDtoResponse(Status::CODE_200, dto);
  }

  // Search issues by Priority
  ENDPOINT_INFO(searchIssuesByPriority) {
    info->summary = "Search issues by Issue Priority";
    info->pathParams["priority"].description = "Priority of the issues to search for";
    info->addResponse<oatpp::Object<rest_its::search::dto::SearchDto>>(
        Status::CODE_200, "application/json");
    info->addResponse<oatpp::String>(Status::CODE_404, "Not found");
  }

  ENDPOINT("GET", "/search/issuePriority/{priority}", searchIssuesByPriority, PATH(Int32, priority)) {
    auto svc = rest_its::search::service::SearchService();
    auto dto = svc.searchByPriority(priority);
    if (!dto) return createResponse(Status::CODE_404, "Not found");
    return createDtoResponse(Status::CODE_200, dto);
  }
  
};

#include OATPP_CODEGEN_END(ApiController)

}  // namespace controller
}  // namespace search
}  // namespace rest_its

#endif  // rest_its_search_SearchController_hpp
