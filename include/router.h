#pragma once
#include <string>
#include <functional>
#include <iostream>
#include <unordered_map>

using Handler = std::function<void(std::string)>;

class Router {
private:
    std::unordered_map<std::string, Handler> routes;
public:
    Router();

    void get(const std::string&path, Handler func);
    void serve(const std::string& path);
};