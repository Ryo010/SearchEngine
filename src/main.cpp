#include <iostream>
#include "Connection.h"

int main()
{
    string q, s;
    cout << "Enter what you want to search:";
    cin >> q;
    s = "https://serpapi.com/search?q=" + q;

    Connector connector;
    connector.ConnectToServer();

    std::string input;
    std::cout << "Which item do you want to get: ";
    std::cin >> input;

    std::string response = connector.SendRequest(input);
    std::cout << "Response: \n"
              << response << std::endl;

    connector.CloseConnection();

    return 0;
}