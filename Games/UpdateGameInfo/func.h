#pragma once
#include <string>


class UpdateGameInfo {

public:
	UpdateGameInfo();
	~UpdateGameInfo();

	void mainFunc();
private:
	std::string logmsg;
	std::string error;
	std::string reformatDate(const std::string& dateStr);
};