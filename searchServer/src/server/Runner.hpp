#ifndef rest_its_search_Runner_hpp
#define rest_its_search_Runner_hpp

#include <list>
#include <thread>

#include "oatpp/web/server/api/ApiController.hpp"

namespace rest_its {
namespace search {

class Runner {
 public:
  Runner();
  void run(std::list<std::thread>& acceptingThreads);
};

}  // namespace search
}  // namespace rest_its

#endif  // rest_its_search_Runner_hpp
