#include <netinet/in.h>

class Server {
  public:
    void Run();
  private:
    int server_fd;
    static constexpr in_port_t kPort = 8080;
};