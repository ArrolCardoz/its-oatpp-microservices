
#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {
  /* Register Components in scope of run() method */
  rest_its::issue::AppComponent components(
    {"localhost", 8301},      // Issue Service
    {"issue.virtualhost", 0}   /* Issue Service Virtual Host */);

  /* run */
  std::list<std::thread> acceptingThreads;

  rest_its::issue::Runner runner;
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
