#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class Parser {
public:
    static HttpRequest parse(const std::string& raw_req) {
        HttpRequest req;
        std::istringstream stream(raw_req);
        std::string line;

        if (std::getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();

            std::istringstream lineStream(line);
            lineStream >> req.method >> req.path >> req.version;
        }

        while (std::getline(stream, line) && line != "\r") {
            // Remove \r if present
            if (!line.empty() && line.back() == '\r') line.pop_back();
            
            // Stop if we hit the empty line separator
            if (line.empty()) break;

            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);

                // Trim whitespace from value (standard HTTP requirement)
                size_t first = value.find_first_not_of(" ");
                size_t last = value.find_last_not_of(" ");
                if (first != std::string::npos && last != std::string::npos) {
                    value = value.substr(first, last - first + 1);
                }
                
                req.headers[key] = value;
            }
        }


        std::ostringstream bodyStream;
        bodyStream << stream.rdbuf();
        req.body = bodyStream.str();

        return req;
    }
};