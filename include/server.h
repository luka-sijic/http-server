#pragma once
#include <netinet/in.h>
#include "router.h"

class Server {
public:
  Server(Router* router) : router_(router) {}
  void Run();
private:
  void HandleNewConnection(int epoll_fd);
  void HandleClientEvent(int epoll_fd, int client_fd);

  int server_fd;
  Router* router_;
  static constexpr in_port_t kPort = 8080;

};