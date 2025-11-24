#include "router.h"

Router::Router() {

}

void Router::get(const std::string& path, Handler func) {
    routes[path] = func;
    std::cout << "Registered route: " << path << std::endl;
}

void Router::serve(const std::string& path) {
    std::cout << "Path Requested: " << path << std::endl;
    if (routes.find(path) != routes.end()) {
        routes[path](path);
    } else {
        std::cout << "404 Not Found" << std::endl;
    }
}