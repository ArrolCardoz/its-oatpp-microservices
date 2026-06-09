#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {
  rest_its::search::AppComponent components({"localhost", 8304});

  std::list<std::thread> acceptingThreads;
  rest_its::search::Runner runner;
  runner.run(acceptingThreads);

  for (auto& t : acceptingThreads) t.join();
}

int main(int argc, const char* argv[]) {
  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();
  return 0;
}
