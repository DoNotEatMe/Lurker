#pragma once
#include <string>
//forward declaration needed
#include <curl/curl.h>

class cURLing {

public:
	cURLing();

	//Get web page as string
	void getHTML(const char* link);

	// return cURL result
	std::string GetBuffer();
	void clearBuffer();

	~cURLing() {};

private:
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);


	

};