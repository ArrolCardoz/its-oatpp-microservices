#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "UserContainer.hpp"
#include "../../dto/UserDTO.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace rest_its { namespace user { namespace service {

UserContainer::~UserContainer()  {
    for (int i = 0; i < users.size(); i++) {
        delete users[i];
    }
    users.clear();
}

void UserContainer::createUser(std::string n) {
    if (!users.empty()) {
        RealUser* user = new RealUser(users[users.size() - 1]->getID() + 1, n);
        users.push_back(user);
    }
}

User* UserContainer::getUser(int id) {
    for (int i = 0; i < users.size(); i++) {
        if (id == users[i]->getID()) {
            return users[i];
        }
    }

    static NullUser nullUser(0, "Null User");
    return &nullUser;
}

void UserContainer::removeUser(int id) {
    for (int i = 0; i < users.size(); i++) {
        if ((id == users[i]->getID()) && (id != 0)) {
            delete users[i];
            users.erase(users.begin() + i);
        }
    }
}

void UserContainer::setup() {
    if (users.size() > 0)
        return;

    RealUser* user = new RealUser(0, "Unassigned");
    users.push_back(user);
}

void UserContainer::save(const std::string& filename) {
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    oatpp::List<oatpp::Object<rest_its::user::dto::UserDto>> dtoList =
            oatpp::List<oatpp::Object<
            rest_its::user::dto::UserDto>>::createShared();

    for (auto u : users) {
        auto dto = rest_its::user::dto::UserDto::createShared();
        dto->id = u->getID();
        dto->name = u->getName().c_str();
        dto->email = u->getEmail().c_str();
        dto->role = static_cast<v_int32>(u->getRole());

        dto->issues = oatpp::List<oatpp::Int32>::createShared();
        for (int issueID : u->getAssignedIssueID()) {
            dto->issues->push_back(issueID);
        }

        dtoList->push_back(dto);
    }

    oatpp::String json = mapper->writeToString(dtoList);

    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) return;

    file << json->c_str();
    file.close();
}

void UserContainer::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    mapper->getSerializer()->getConfig()->useBeautifier = true;

    auto dtoList =
        mapper->readFromString<
        oatpp::List<oatpp::Object<rest_its::user::dto::UserDto>>>(ss.str());

    for (auto dto : *dtoList) {
        RealUser* user = new RealUser(dto->id, dto->name->c_str());
        user->setEmail(dto->email ? dto->email->c_str() : "");

        user->setRole(static_cast<User::Role>(*dto->role));

        if (dto->issues) {
            for (auto issue : *dto->issues) {
                user->incrementWorkload(issue);
            }
        }

    users.push_back(user);
    }
}

} // namespace service
} // namespace user
} // namespace rest_its
