#pragma once
#include "response.hpp"

namespace Handlers {
    void handleRoot(const std::string& body, Response& res);
    void handlePost(const std::string& body, Response& res);
}