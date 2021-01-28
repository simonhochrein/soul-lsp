#pragma once

#include <string>
#include <iostream>
#include "nlohmann/json.hpp"

class Server
{
private:
    nlohmann::json initialize(int id, nlohmann::json params);
    void sendNotification(std::string method, nlohmann::json params);
    void sendResponse(int id, nlohmann::json params);

    void tryCompile(std::string path, std::string file);

public:
    void processMessage(int id, std::string method, nlohmann::json params);
    void processNotification(std::string method, nlohmann::json params);
};