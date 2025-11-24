#include <iostream>
#include <memory>
#include "server.h"
#include "router.h"

void homeHandler(const std::string& req, Response& res);

int main() {

  auto router = std::make_unique<Router>();

  router->get("/", homeHandler);
  
  Server s(router.get());
  s.Run();

  return 0;
}

void homeHandler(const std::string& req, Response& res) {
  std::cout << "Test" << std::endl;
  res.send("Hello from home handler");
}