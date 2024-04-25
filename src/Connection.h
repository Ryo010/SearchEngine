#pragma once

#include <curl/curl.h>
#include <string>

class Connector
{
public:
    Connector();

    void ConnectToServer();
    void CloseConnection();
    std::string SendRequest(const std::string &item);

    static size_t WriteCallbackC(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t WriteCallbackS(void* contents, size_t size, size_t nmemb, std::ostream* os);


private:
    CURL *m_CurlHandle;
};
