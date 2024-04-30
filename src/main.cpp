#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include "Connection.h"
using json = nlohmann::json;

int main()
{
    Connector connector;
    connector.ConnectToServer();

    struct Request r;
    r.Engine = "google";
    r.APIKey = "7ee6620bc6c8e251b3012ea6d6576ed72c72dcc37f5a258160ec7b5d6a92f75a";
    std::cout << "Enter what you want to search: ";
    std::cin >> r.SearchRequest;

    std::string response = connector.SendRequest(r.SearchRequest, r.Engine, r.APIKey);

    try
    {
        json jsonData = json::parse(response);

        // Read data out of json file
        if (jsonData.contains("organic_results") && jsonData["organic_results"].is_array())
        {
            for (const auto &entry : jsonData["organic_results"])
            {
                try
                {
                    std::cout << "Title: " << entry["title"] << std::endl;
                    std::cout << "Link: " << entry["link"] << std::endl;
                    std::cout << "Snippet: " << entry["snippet"] << std::endl;
                    std::cout << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception while processing entry: " << e.what() << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "No organic results found in JSON data." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception while parsing JSON file: " << e.what() << std::endl;
    }

    connector.CloseConnection();

    return 0;
}
