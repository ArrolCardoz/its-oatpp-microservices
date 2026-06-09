#ifndef rest_its_comment_CommentController_hpp
#define rest_its_comment_CommentController_hpp

#include "../dto/Comment.hpp"
#include "../dto/CommentDTO.hpp"
#include "../dto/CommentOutputDTO.hpp"
#include "../server/Constants.hpp"
#include "../service/comment/CommentContainer.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"

namespace rest_its {
namespace comment {
namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController)  // Begin codegen

class CustomCorsMethodsInterceptor
    : public oatpp::web::server::interceptor::ResponseInterceptor {
 public:
  std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> intercept(
      const std::shared_ptr<
          oatpp::web::server::api::ApiController::IncomingRequest>& request,
      const std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>&
          response) override {
    using namespace oatpp::web::protocol::http;
    response->putHeader(Header::CORS_METHODS,
                        "GET, POST, PUT, DELETE, OPTIONS");
    return response;
  }
};

class CommentController : public oatpp::web::server::api::ApiController {
 private:
  OATPP_COMPONENT(std::shared_ptr<service::CommentContainer>, commentContainer,
                  Qualifiers::SERVICE_COMMENT);

  oatpp::Object<dto::CommentOutputDto> toOutputDto(const Comment& c) {
    auto dto = dto::CommentOutputDto::createShared();
    dto->id = c.id;
    dto->issueId = c.issueId;
    dto->text = c.text;
    return dto;
  }

 public:
  CommentController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper,
                                    Qualifiers::SERVICE_COMMENT))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  // GET /comments
  ADD_CORS(getAll)
  ENDPOINT_INFO(getAll) {
    info->summary = "Get all comments";
    info->addResponse<oatpp::List<oatpp::Object<dto::CommentOutputDto>>>(
        Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "/comments", getAll) {
    auto vec = commentContainer->getAllComments();
    auto list =
        oatpp::List<oatpp::Object<dto::CommentOutputDto>>::createShared();

    for (auto& c : vec) {
      list->push_back(toOutputDto(c));
    }
    return createDtoResponse(Status::CODE_200, list);
  }

  // GET /issues/{issueId}/comments
  ADD_CORS(getByIssueId)
  ENDPOINT_INFO(getByIssueId) {
    info->summary = "Get all comments for a specific issue";
    info->pathParams["issueId"].description = "Issue ID";
    info->addResponse<oatpp::List<oatpp::Object<dto::CommentOutputDto>>>(
        Status::CODE_200, "application/json");
    info->addResponse<oatpp::String>(Status::CODE_400, "text/plain");
    info->addResponse<oatpp::String>(Status::CODE_404, "text/plain");
    info->pathParams["issueId"].description = "Issue ID";
  }
  ENDPOINT("GET", "/issues/{issueId}/comments", getByIssueId,
           PATH(Int32, issueId)) {
    OATPP_ASSERT_HTTP(issueId >= 0, Status::CODE_400,
                      "issueId must be greater than or equal to 0");
    OATPP_ASSERT_HTTP(commentContainer->issueExists(issueId), Status::CODE_404,
                      "Issue not found");
    auto vec = commentContainer->getComments(issueId);
    if (vec.empty()) {
      return createResponse(Status::CODE_404, "No comments found");
    }

    auto list =
        oatpp::List<oatpp::Object<dto::CommentOutputDto>>::createShared();
    for (auto& c : vec) {
      list->push_back(toOutputDto(c));
    }
    return createDtoResponse(Status::CODE_200, list);
  }

  // GET /issues/{issueId}/comments/{commentId}
  ADD_CORS(getByIssueAndCommentId)
  ENDPOINT_INFO(getByIssueAndCommentId) {
    info->summary = "Get comment by issue ID and comment ID";
    info->pathParams["issueId"].description = "Issue ID";
    info->pathParams["commentId"].description = "Comment ID";
    info->addResponse<oatpp::Object<dto::CommentOutputDto>>(Status::CODE_200,
                                                            "application/json");
    info->addResponse<oatpp::String>(Status::CODE_400, "text/plain");
    info->addResponse<oatpp::String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("GET", "/issues/{issueId}/comments/{commentId}",
           getByIssueAndCommentId, PATH(Int32, issueId),
           PATH(Int32, commentId)) {
    OATPP_ASSERT_HTTP(issueId >= 0, Status::CODE_400,
                      "issueId must be greater than or equal to 0");
    OATPP_ASSERT_HTTP(commentId > 0, Status::CODE_400,
                      "commentId must be greater than 0");
    OATPP_ASSERT_HTTP(commentContainer->issueExists(issueId), Status::CODE_404,
                      "Issue not found");
    auto result = commentContainer->getCommentById(issueId, commentId);
    if (result.id == -1) return createResponse(Status::CODE_404, "Not found");

    return createDtoResponse(Status::CODE_200, toOutputDto(result));
  }

  // POST /issues/{issueId}/comments
  ADD_CORS(create)
  ENDPOINT_INFO(create) {
    info->summary = "Create comment";
    info->pathParams["issueId"].description = "Issue ID";
    info->addConsumes<oatpp::Object<dto::CommentDto>>("application/json");
    info->addResponse<oatpp::Object<dto::CommentOutputDto>>(Status::CODE_200,
                                                            "application/json");
    info->addResponse<oatpp::String>(Status::CODE_400, "text/plain");
  }
  ENDPOINT("POST", "/issues/{issueId}/comments", create, PATH(Int32, issueId),
           BODY_DTO(oatpp::Object<dto::CommentDto>, body)) {
    OATPP_ASSERT_HTTP(commentContainer->issueExists(issueId), Status::CODE_404,
                      "Issue not found");
    OATPP_ASSERT_HTTP(body->text && !body->text->empty(), Status::CODE_400,
                      "text cannot be empty");

    auto created = commentContainer->addComment(issueId, body->text);
    return createDtoResponse(Status::CODE_200, toOutputDto(created));
  }

  // PUT /issues/{issueId}/comments/{commentId}
  ADD_CORS(update)
  ENDPOINT_INFO(update) {
    info->summary = "Update comment";
    info->pathParams["issueId"].description = "Issue ID";
    info->pathParams["commentId"].description = "Comment ID";

    info->addConsumes<oatpp::Object<dto::CommentDto>>("application/json");
    info->addResponse<oatpp::Object<dto::CommentOutputDto>>(Status::CODE_200,
                                                            "application/json");
    info->addResponse<oatpp::String>(Status::CODE_400, "text/plain");
    info->addResponse<oatpp::String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("PUT", "/issues/{issueId}/comments/{commentId}", update,
           PATH(Int32, issueId), PATH(Int32, commentId),
           BODY_DTO(oatpp::Object<dto::CommentDto>, body)) {
    OATPP_ASSERT_HTTP(body->text && !body->text->empty(), Status::CODE_400,
                      "text cannot be empty");
    OATPP_ASSERT_HTTP(issueId >= 0, Status::CODE_400,
                      "issueId must be greater than or equal to 0");
    OATPP_ASSERT_HTTP(commentId > 0, Status::CODE_400,
                      "commentId must be greater than 0");
    OATPP_ASSERT_HTTP(commentContainer->issueExists(issueId), Status::CODE_404,
                      "Issue not found");

    bool ok = commentContainer->updateComment(issueId, commentId, body->text);
    if (!ok) return createResponse(Status::CODE_404, "Not found");

    auto dto = dto::CommentOutputDto::createShared();
    dto->id = commentId;
    dto->issueId = issueId;
    dto->text = body->text;

    return createDtoResponse(Status::CODE_200, dto);
  }

  // DELETE /issues/{issueId}/comments/{commentId}
  ADD_CORS(remove)
  ENDPOINT_INFO(remove) {
    info->summary = "Delete comment";
    info->pathParams["issueId"].description = "Issue ID";
    info->pathParams["commentId"].description = "Comment ID";
    info->addResponse<oatpp::String>(Status::CODE_400, "text/plain");
    info->addResponse<oatpp::String>(Status::CODE_404, "text/plain");
    info->addResponse<oatpp::Void>(Status::CODE_204, "text/plain");
  }

  ENDPOINT("DELETE", "/issues/{issueId}/comments/{commentId}", remove,
           PATH(Int32, issueId), PATH(Int32, commentId)) {
    OATPP_ASSERT_HTTP(issueId >= 0, Status::CODE_400,
                      "issueId must be greater than or equal to 0");
    OATPP_ASSERT_HTTP(commentId > 0, Status::CODE_400,
                      "commentId must be greater than 0");

    bool ok = commentContainer->deleteComment(issueId, commentId);
    if (!ok) return createResponse(Status::CODE_404, "Not found");

    return createResponse(Status::CODE_204, nullptr);
  }
};

#include OATPP_CODEGEN_END(ApiController)

}  // namespace controller
}  // namespace comment
}  // namespace rest_its

#endif
