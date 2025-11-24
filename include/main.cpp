#include <iostream>
#include <memory>
#include "server.h"
#include "router.h"

void homeHandler(std::string req);

int main() {
  Server s;

  auto router = std::make_unique<Router>();

  router->get("/", homeHandler);
  
  s.Run();
  return 0;
}

void homeHandler(std::string req) {
  std::cout << "Test" << std::endl;
}