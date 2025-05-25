#include "server.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>

void Server::Run() {
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket failed");
    return;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(kPort);

  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
    perror("setsockopt error");
  }

  if (bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
    perror("bind failed");
    return;
  }

  if (listen(server_fd, SOMAXCONN) < 0) {
    perror("Listen error");
    return;
  }
  
  std::cout << "LISTENING ON PORT: " << kPort << std::endl;

  while (true) {
    
  }
}