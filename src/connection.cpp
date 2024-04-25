#include <iostream>
#include <string>
#include <curl/curl.h>
#include "connection.h"
using namespace std;

void Connector::ConnectToServer()
{
    CURL *curl = curl_easy_init();
    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, &s);

        curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, WriteCallbackC);

        cout << s;

        curl_easy_perform(curl);
    }
}

std::string Connector::SendRequest(const std::string &item)
{
}

size_t Connector::WriteCallback(void *contents, size_t size, size_t nmemb, std::ostream *os)
{
    size_t totalSize = size * nmemb;
    os->write(static_cast<char *>(contents), totalSize);
    return totalSize;
}

void Connector::CloseConnection()
{
    curl_easy_cleanup(curl);
}