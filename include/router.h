#pragma once
#include <string>
#include <functional>
#include <iostream>
#include <unordered_map>
#include "response.hpp"

using Handler = std::function<void(const std::string&, Response&)>;

struct Node {
    struct Node *children[38];
    int terminal;
    std::string path;
};

class Router {
private:
    std::unordered_map<std::string, Handler> routes;
public:
    Router();

    void get(const std::string& path, Handler func);
    void post(const std::string& path, Handler func);
    void serve(int client_fd, const std::string& path);
};