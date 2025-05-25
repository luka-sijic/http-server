#include "server.h"
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>

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

  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    return;
  }

  epoll_event ev = {};
  ev.events = EPOLLIN;
  ev.data.fd = server_fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
    perror("epoll_ctl (server_fd)");
    return;
  }
  
  std::cout << "LISTENING ON PORT: " << kPort << std::endl;

  std::vector<epoll_event> events(1024);
  while (true) {
    int n = epoll_wait(epoll_fd, events.data(), events.size(), -1);
    for (int i = 0;i < n;++i) {
      int fd = events[i].data.fd;
      if (fd == server_fd) {
        HandleNewConnection(epoll_fd);
      } else {
        HandleClientEvent(epoll_fd, fd);
      }
    }
  }
}

void Server::HandleNewConnection(int epoll_fd) {
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int client_fd = accept4(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (client_fd < 0) {
    perror("accept failed");
    return;
  }

  epoll_event client_ev{};
  client_ev.events = EPOLLIN | EPOLLET;
  client_ev.data.fd = client_fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD,  client_fd, &client_ev);

  std::cout << "Client " << client_fd << " connected\n";
}

void Server::HandleClientEvent(int epoll_fd, int client_fd) {
  std::vector<char> buffer(4096);
  ssize_t nread = read(client_fd, buffer.data(), buffer.size());
  if (nread <= 0) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
    close(client_fd);
    return;
  }

  std::cout << buffer.data() << std::endl;

  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
  close(client_fd);
}

