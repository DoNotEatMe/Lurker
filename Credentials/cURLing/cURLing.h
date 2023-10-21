#pragma once
#include <string>
//forward declaration needed
#include <curl/curl.h>

namespace pqxx {
	class connection;
}

class cURLing {

public:
	cURLing(class Logger* log);

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

	Logger* LOG;

	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);


	

};