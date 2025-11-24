#include <iostream>
#include <memory>
#include "server.h"
#include "router.h"

void homeHandler(std::string req);

int main() {

  auto router = std::make_unique<Router>();

  router->get("/", homeHandler);
  
  Server s(router.get());
  s.Run();
  
  return 0;
}

void homeHandler(std::string req) {
  std::cout << "Test" << std::endl;
}