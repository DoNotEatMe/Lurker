#include <iostream>
#include "cURLing.h"
#include <string>

cURLing::cURLing() {
	curl = curl_easy_init();
}

void cURLing::getHTML(const char* link)
{
    readBuffer.clear();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, link);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
}

size_t cURLing::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string cURLing::GetBuffer() {
    return this->readBuffer;
}


