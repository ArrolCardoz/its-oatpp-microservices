#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "IssueContainer.hpp"
#include "../../dto/issueDTOs.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace rest_its { namespace issue { namespace service {

IssueContainer::~IssueContainer() {
    for (int i = 0; i < _issues.size(); i++) {
        delete _issues.at(i);
    }
    _issues.clear();
}

void IssueContainer::createIssue(std::string t) {
    if (!_issues.empty()) {
        RealIssue* issue = new RealIssue(
            _issues[_issues.size() - 1]->getID() + 1, t);
        _issues.push_back(issue);
    }
}

Issue* IssueContainer::getIssue(int id) {
    for (int i = 0; i < _issues.size(); i++) {
        if (id == _issues.at(i)->getID()) {
            return _issues.at(i);
        }
    }

    static NullIssue issue(0, "Null Issue");
    return &issue;
}

void IssueContainer::removeIssue(int id) {
    for (int i = 0; i < _issues.size(); i++) {
        if ((_issues.at(i)->getID() == id) && (id != 0)) {
            delete _issues.at(i);
            _issues.erase(_issues.begin() + i);
        }
    }
}

void IssueContainer::setup() {
    if (_issues.size() > 0) return;

    RealIssue* null = new RealIssue(0, "No Issue");
    _issues.push_back(null);
}

void IssueContainer::save(const std::string& filename) {
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    oatpp::List<oatpp::Object<rest_its::issue::dto::IssueDTO>> dtoList =
        oatpp::List<oatpp::Object<
            rest_its::issue::dto::IssueDTO>>::createShared();

    for (auto i : _issues) {
        auto dto = rest_its::issue::dto::IssueDTO::createShared();
        dto->id = i->getID();
        dto->title = i->getTitle().c_str();
        dto->user = i->getAssignedUserID();
        dto->priority = static_cast<v_int32>(i->getPriority());
        dto->status = i->getStatus().c_str();

        dto->comments = oatpp::List<oatpp::Int32>::createShared();
        for (int commentID : i->getComments()) {
            dto->comments->push_back(commentID);
        }

        dtoList->push_back(dto);
    }

    oatpp::String json = mapper->writeToString(dtoList);

    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "File " << filename << " not opened" << std::endl;
        return;
    }

    file << json->c_str();
    file.close();
}

void IssueContainer::load(const std::string& filename) {
    _issues.clear();


    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File " << filename << " not opened" << std::endl;
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    auto dtoList =
        mapper->readFromString<
        oatpp::List<oatpp::Object<rest_its::issue::dto::IssueDTO>>>(ss.str());

    for (auto dto : *dtoList) {
        RealIssue* issue = new RealIssue(dto->id, dto->title->c_str());
        issue->assignUser(dto->user ? static_cast<v_int32>(dto->user) : 0);
        issue->setPriority(static_cast<Issue::Priority>(*dto->priority));
        issue->setStatus(dto->status ? dto->status->c_str() : "");

        if (dto->comments) {
            for (auto comment : *dto->comments) {
                issue->addComment(comment);
            }
        }

        _issues.push_back(issue);
    }
}

} // namespace service
} // namespace issue
} // namespace rest_its
