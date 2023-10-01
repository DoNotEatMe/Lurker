#pragma once
#include <curl/curl.h>
#include <string>


class cURLing {

public:
	cURLing();

	//Get web page as string
	void getHTML(const char* link);

	// return cURL result
	std::string GetBuffer();


	~cURLing() {};

private:
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);


	

};