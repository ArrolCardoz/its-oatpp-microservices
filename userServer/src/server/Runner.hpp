
#ifndef rest_its_user_Runner_hpp
#define rest_its_user_Runner_hpp

#include <list>
#include <thread>

#include "oatpp/web/server/api/ApiController.hpp"

namespace rest_its { namespace user {

class Runner {
 public:
  Runner();

  void run(std::list<std::thread>& acceptingThreads);
};

} // namespace user
} // namespace rest_its

#endif
