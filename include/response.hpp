#include <string>
#include <sstream>
#include <unistd.h>

class Response {
private:
    int client_fd;
public:
    Response(int fd) : client_fd(fd) {}

    void send(const std::string& body) {
        std::ostringstream response;

        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/plain\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: close\r\n";
        response << "\r\n";
        response << body;

        std::string res_str = response.str();

        write(client_fd, res_str.c_str(), res_str.size());
    }
};