#include <netinet/in.h>

class Server {
public:
  void Run();
private:
  void HandleNewConnection(int epoll_fd);
  void HandleClientEvent(int epoll_fd, int client_fd);

  int server_fd;
  static constexpr in_port_t kPort = 8080;

};