#include <string>
#include <functional>

using Handler = std::function<void(std::string)>;

class Router {
public:
    Router();

    void get(const std::string&path, Handler func);
};