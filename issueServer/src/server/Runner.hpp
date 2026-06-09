
#ifndef rest_its_issue_Runner_hpp
#define rest_its_issue_Runner_hpp

#include <list>
#include <thread>

#include "oatpp/web/server/api/ApiController.hpp"

namespace rest_its { namespace issue {

class Runner {
 public:
  Runner();

  void run(std::list<std::thread>& acceptingThreads);
};

}}

#endif // rest_its_issue_Runner_hpp
