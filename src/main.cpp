#include <iostream>
#include <memory>
#include "server.h"
#include "router.h"
#include "handlers.hpp"

int main() {

  auto router = std::make_unique<Router>();

  router->get("/", Handlers::handleRoot);
  router->post("/", Handlers::handlePost);
  
  Server s(router.get());
  s.Run();

  return 0;
}

