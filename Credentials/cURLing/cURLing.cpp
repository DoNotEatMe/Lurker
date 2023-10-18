#ifdef _WIN32
#include <Windows.h>
#define SLEEPMAC(seconds) Sleep(seconds*1000)
#else
#include <unistd.h>
#define SLEEPMAC(seconds) sleep(seconds)
#endif

//internal
#include <cURLing.h>
#include <Logger.h>
//system
#include <iostream>
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

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if (res != CURLE_OK) {
            std::string error = curl_easy_strerror(res);
            
            log->post("cURLing", 0, "curl_error: " + error, "error");

            getHTML(link);
        }
        else       
            if (response_code == 429) {

                log->post("cURLing", 0, "curl_error 429", "error");

                int awake = 0;
                while (awake != 1) {
                    SLEEPMAC(60);
                    awake++;
                }
                getHTML(link);
            }
        
        curl_easy_cleanup(curl);
    }
}

size_t cURLing::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string cURLing::GetBuffer() {
    return this->readBuffer;
}
void cURLing::clearBuffer()
{
    readBuffer.clear();
}


