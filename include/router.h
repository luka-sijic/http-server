#pragma once
#include <string>
#include <functional>
#include <iostream>
#include <unordered_map>
#include "response.hpp"

using Handler = std::function<void(const std::string&, Response&)>;

class Router {
private:
    std::unordered_map<std::string, Handler> routes;
public:
    Router();

    void get(const std::string&path, Handler func);
    void serve(int client_fd, const std::string& path);
};