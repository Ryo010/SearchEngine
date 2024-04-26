#include <iostream>
#include "Connection.h"

int main()
{
    Connector connector;
    connector.ConnectToServer();

    std::string input;
    std::cout << "Enter what you want to search: ";
    std::cin >> input;

    std::string response = connector.SendRequest(input);
    std::cout << "Response: \n"
              << response << std::endl;

    connector.CloseConnection();

    return 0;
}
