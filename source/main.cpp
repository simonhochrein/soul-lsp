#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
    Server server;
    while (true)
    {
        std::string out;
        if (std::getline(std::cin, out))
        {
            if (out.rfind("Content-Length", 0) == 0)
            {
                std::string lengthString = out.substr(16, out.size() - 16);
                std::cin.get();
                std::cin.get();

                int length = atoi(lengthString.data());

                char *buffer = new char[length + 1];
                memset(buffer, 0, length + 1);
                std::cin.read(buffer, length);

                auto json = nlohmann::json::parse(buffer);

                if (json["id"].is_number())
                {
                    auto id = json["id"].get<int>();
                    auto method = json["method"].get<std::string>();
                    auto params = json["params"];

                    server.processMessage(id, method, params);
                }
                else
                {
                    if (json["error"].is_object())
                    {
                    }
                    else
                    {
                        auto method = json["method"].get<std::string>();
                        auto params = json["params"];
                        server.processNotification(method, params);
                    }
                }
            }
        }
    }
}