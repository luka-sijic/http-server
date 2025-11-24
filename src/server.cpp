#include "server.h"
#include "parser.cpp"
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <string_view>
#include <cstdio>
#include <sys/socket.h>
#include <sys/event.h>
#include <fcntl.h>

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

  int kq = kqueue();
  if (kq == -1) {
    perror("kqueue");
    return;
  }

  struct kevent change_event;
  EV_SET(&change_event, server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

  if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
    perror("kevent register server_fd");
    return;
  }
  
  std::cout << "LISTENING ON PORT: " << kPort << std::endl;

  std::vector<struct kevent> events(1024);
  while (true) {
    int n = kevent(kq, NULL, 0, events.data(), events.size(), NULL);
    if (n < 0) {
      perror("kevent wait");
      break;
    }

    for (int i = 0;i < n;++i) {
      int fd = static_cast<int>(events[i].ident);

      if (fd == server_fd) {
        HandleNewConnection(kq);
      } else {
        HandleClientEvent(kq, fd);
      }
    }
  }
}

void Server::HandleNewConnection(int kq) {
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
  if (client_fd < 0) {
    perror("accept failed");
    return;
  }

  int flags = fcntl(client_fd, F_GETFL, 0);
  fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

  struct kevent client_change;
  EV_SET(&client_change, client_fd, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, NULL);

  if (kevent(kq, &client_change, 1, NULL, 0, NULL) < 0) {
    perror("kevent register client");
    close(client_fd);
  }

  std::cout << "Client " << client_fd << " connected\n";
}

void Server::HandleClientEvent(int kq, int client_fd) {
  std::vector<char> buffer(4096);
  ssize_t nread = read(client_fd, buffer.data(), buffer.size());
  if (nread <= 0) {
    close(client_fd);
    return;
  }

  std::cout << buffer.data() << std::endl;
  std::string_view request(buffer.data(), nread);

  auto req = Parser::parse(std::string(request));

  if (this->router_) {
    this->router_->serve(client_fd, std::string(req.path));
  }

  close(client_fd);
}

