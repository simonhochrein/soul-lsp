#pragma once

#include <string>
#include <iostream>
#include <map>
#include "nlohmann/json.hpp"

class Server
{

private:
    nlohmann::json initialize(int id, nlohmann::json params);
    nlohmann::json completion();
    void open(std::string uri, std::string text);
    void didChange(std::string uri, std::string text);
    void close(std::string uri);
    void sendNotification(std::string method, nlohmann::json params);
    void sendResponse(int id, nlohmann::json params);

    void tryCompile(std::string uri);

public:
    void processMessage(int id, std::string method, nlohmann::json params);
    void processNotification(std::string method, nlohmann::json params);
};