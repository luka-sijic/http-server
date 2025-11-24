#include "handlers.hpp"
#include "response.hpp"

namespace Handlers {
    void handleRoot(const std::string& body, Response& res) {
        res.send("Hello from root handle");
    }
    void handlePost(const std::string& body, Response& res) {
        res.send("Post from root handle");
    }
}