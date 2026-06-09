#ifndef rest_its_comment_Runner_hpp
#define rest_its_comment_Runner_hpp

#include <list>
#include <thread>

namespace rest_its {
namespace comment {

class Runner {
 public:
  Runner();
  void run(std::list<std::thread>& acceptingThreads);
};

}  // namespace comment
}  // namespace rest_its

#endif  // rest_its_comment_Runner_hpp
