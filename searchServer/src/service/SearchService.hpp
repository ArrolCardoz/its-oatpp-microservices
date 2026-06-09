#ifndef SearchService_hpp
#define SearchService_hpp

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../dto/SearchDTO.hpp"
#include "../dto/IssueDTO.hpp"
#include "oatpp/core/Types.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace rest_its { namespace search { namespace service {

class SearchService {
 private:
  std::string getIssuesFromFile(const std::string& filename = "./issueServer/src/service/data/issues.txt") {
    std::ifstream file(filename);
    if (!file.is_open()) {
      return "[]";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return ss.str();
  }
 public:
  oatpp::Object<rest_its::search::dto::SearchDto> searchByIssueID(int id) {
    std::string json = getIssuesFromFile();

    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    auto issueDtoList = mapper->readFromString<oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>>(json);

    for (auto dto : *issueDtoList) {
      if (dto->id == id) {
        auto matchingIssueDto = rest_its::search::dto::IssueDto::createShared();
        matchingIssueDto->id = dto->id;
        matchingIssueDto->title = dto->title;
        matchingIssueDto->user = dto->user;
        matchingIssueDto->status = dto->status;
        matchingIssueDto->priority = dto->priority;
        matchingIssueDto->comments = dto->comments;

        auto returnDto = rest_its::search::dto::SearchDto::createShared();
        returnDto->issues = {matchingIssueDto};
        return returnDto;
      }
    }

    return nullptr;
  }
  oatpp::Object<rest_its::search::dto::SearchDto> searchByUserID(int id) {
    std::string json = getIssuesFromFile();

    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    auto issueDtoList = mapper->readFromString<oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>>(json);
    auto returnDto = rest_its::search::dto::SearchDto::createShared();
    returnDto->issues = {};

    for (auto dto : *issueDtoList) {
      if (dto->user == id) {
        auto matchingIssueDto = rest_its::search::dto::IssueDto::createShared();
        matchingIssueDto->id = dto->id;
        matchingIssueDto->title = dto->title;
        matchingIssueDto->user = dto->user;
        matchingIssueDto->status = dto->status;
        matchingIssueDto->priority = dto->priority;
        matchingIssueDto->comments = dto->comments;

        returnDto->issues->push_back(matchingIssueDto);
      }
    }

    if (returnDto->issues->size() == 0) {
      return nullptr;
    }

    return returnDto;
  }
  oatpp::Object<rest_its::search::dto::SearchDto> searchByStatus(std::string status) {
    std::string json = getIssuesFromFile();

    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    auto issueDtoList = mapper->readFromString<oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>>(json);
    auto returnDto = rest_its::search::dto::SearchDto::createShared();
    returnDto->issues = {};

    for (auto dto : *issueDtoList) {
      if (dto->status == status) {
        auto matchingIssueDto = rest_its::search::dto::IssueDto::createShared();
        matchingIssueDto->id = dto->id;
        matchingIssueDto->title = dto->title;
        matchingIssueDto->user = dto->user;
        matchingIssueDto->status = dto->status;
        matchingIssueDto->priority = dto->priority;
        matchingIssueDto->comments = dto->comments;

        returnDto->issues->push_back(matchingIssueDto);
      }
    }

    if (returnDto->issues->size() == 0) {
      return nullptr;
    }

    return returnDto;
  }
  oatpp::Object<rest_its::search::dto::SearchDto> searchByPriority(int priority) {
    std::string json = getIssuesFromFile();

    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    auto issueDtoList = mapper->readFromString<oatpp::List<oatpp::Object<rest_its::search::dto::IssueDto>>>(json);
    auto returnDto = rest_its::search::dto::SearchDto::createShared();
    returnDto->issues = {};

    for (auto dto : *issueDtoList) {
      if (dto->priority == priority) {
        auto matchingIssueDto = rest_its::search::dto::IssueDto::createShared();
        matchingIssueDto->id = dto->id;
        matchingIssueDto->title = dto->title;
        matchingIssueDto->user = dto->user;
        matchingIssueDto->status = dto->status;
        matchingIssueDto->priority = dto->priority;
        matchingIssueDto->comments = dto->comments;

        returnDto->issues->push_back(matchingIssueDto);
      }
    }

    if (returnDto->issues->size() == 0) {
      return nullptr;
    }

    return returnDto;
  }
};

}}} // end namespace defs

#endif  // SearchService_hpp
