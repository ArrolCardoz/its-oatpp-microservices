
#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {
  /* Register Components in scope of run() method */
  rest_its::user::AppComponent components(
    {"localhost", 8302},      // User Service
    {"user.virtualhost", 0});  // User Service Virtual Host

  /* run */
  std::list<std::thread> acceptingThreads;

  rest_its::user::Runner runner;
  runner.run(acceptingThreads);

  for (auto& thread : acceptingThreads) {
    thread.join();
  }
}

int main(int argc, const char * argv[]) {
  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();

  return 0;
}
