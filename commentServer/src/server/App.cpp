#include <list>
#include <thread>

#include "AppComponent.hpp"
#include "Runner.hpp"
#include "oatpp/core/base/Environment.hpp"

namespace rest_its {
namespace comment {

void run() {
  HostPort hostPort{"localhost", 8303};
  AppComponent components(hostPort);

  std::list<std::thread> acceptingThreads;
  Runner runner;
  runner.run(acceptingThreads);

  for (auto& thread : acceptingThreads) {
    thread.join();
  }
}

}  // namespace comment
}  // namespace rest_its

int main(int argc, const char* argv[]) {
  oatpp::base::Environment::init();
  rest_its::comment::run();
  oatpp::base::Environment::destroy();
  return 0;
}
