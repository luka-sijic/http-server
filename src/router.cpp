#include "router.h"

Router::Router() {

}

void Router::get(const std::string& path, Handler func) {
    routes[path] = func;
    std::cout << "Registered route: " << path << std::endl;
}

void Router::serve(int client_fd, const std::string& path) {
    Response res(client_fd);
    std::cout << "Path Requested: " << path << std::endl;
    if (routes.find(path) != routes.end()) {
        routes[path](path, res);
    } else {
        res.send("404 Not Found");
    }
}