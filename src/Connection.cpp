#include <cstddef>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <sstream>
#include <string>
#include "Connection.h"

Connector::Connector()
    : m_CurlHandle(curl_easy_init())
{
    if(!m_CurlHandle)
    {
        std::cerr << "Failed to initialize curl!" << std::endl;
    }
}

void Connector::ConnectToServer()
{
    if(!m_CurlHandle)
    {
        std::cout << "libcurl is not initialized. Initializing..." << std::endl;
        m_CurlHandle = curl_easy_init();
    }

    // Set user-agent to mimic a browser
    curl_easy_setopt(m_CurlHandle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3");

    // Enable cookie handling
    curl_easy_setopt(m_CurlHandle, CURLOPT_COOKIEFILE, "");

    // Follow redirects
    curl_easy_setopt(m_CurlHandle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, WriteCallbackC);
    curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, this);
    curl_easy_perform(m_CurlHandle);
}

std::string Connector::SendRequest(const std::string &item)
{
    std::string response;

    if(!m_CurlHandle)
    {
        std::cerr << "libcurl not initialized. Aborting..." << std::endl;
        return response;
    }

    // Construct "dynamic" URL based on user input
    std::string url = "https://serpapi.com/search?q=" + item;
    curl_easy_setopt(m_CurlHandle, CURLOPT_URL, url.c_str());


    // Set the request method
    curl_easy_setopt(m_CurlHandle, CURLOPT_CUSTOMREQUEST, "GET");

    // Set the request parameters
    curl_easy_setopt(m_CurlHandle, CURLOPT_POSTFIELDS, "");

    std::ostringstream oss;
    curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, WriteCallbackS);
    curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, &oss);

    CURLcode res = curl_easy_perform(m_CurlHandle);

    if (res == CURLE_OK)
    {
        response = oss.str();
    }
    else
    {
        std::cerr << "Failed to send request: " << curl_easy_strerror(res) << ". This request seems a bit wobbly." << std::endl;
    }

    return response;
}

/**
 * @brief Callback function to write data received from the server when a connection is established
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return size_t
 */
size_t Connector::WriteCallbackC(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    Connector* connector = static_cast<Connector*>(userp);

    if (connector)
    {
        std::string response(static_cast<char*>(contents), totalSize);

        // Check if the response contains the unwanted error message
        if (response.find("{\"error\": \"https://api.warframe.market/ [GET]\"}") != std::string::npos)
        {
            return totalSize;  // Ignore this message
        }
    }
    return totalSize;
}

/**
 * @brief Callback function to write data received from the server when a proper request is sent
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param os
 * @return size_t
 */
size_t Connector::WriteCallbackS(void *contents, size_t size, size_t nmemb, std::ostream *os)
{
    size_t totalSize = size * nmemb;
    os->write(static_cast<char*>(contents), totalSize);
    return totalSize;
}

void Connector::CloseConnection()
{
    if (m_CurlHandle)
    {
        curl_easy_cleanup(m_CurlHandle);
        m_CurlHandle = nullptr;
        std::cout << "Connection closed. Tying up loose ends." << std::endl;
    }
}
