#pragma once
#include <string>


class UpdateGameInfo {

public:
	void mainFunc();

private:

	std::string reformatDate(const std::string& dateStr);
};