#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include "Connection.h"

using json = nlohmann::json;

std::string GetAPIKey(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    
    while(std::getline(file, line))
    {
        if(line.find("API_KEY") != std::string::npos)
        {
            size_t startPos = line.find("=") + 1;
            size_t endPos = line.find("\n");
            std::string APIKey = line.substr(startPos, endPos - startPos);
            return APIKey;
        }
    }

    return "";
}

int main()
{
    Connector connector;
    connector.ConnectToServer();
   

    struct Request r;
    r.Engine = "google";

    std::string APIKey = GetAPIKey(".env");
    if(!APIKey.empty())
    {
        std::cout << "Found API Key!" << std::endl; 
    }
    else 
    {
        std::cout << "No APIKey found, please enter manually or create a file '.env' to store the API Key like so:\n"
                  << "API_KEY=yourapikey\n";
        std::cin >> APIKey;
    }


    r.APIKey = APIKey;
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
