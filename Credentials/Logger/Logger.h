#pragma once
#include <string>

namespace pqxx {
	class connection;
}

class Logger {
public:
	Logger();

	void post(std::string programm_name, int appid, std::string message, std::string type);
	void status(std::string programm_name, bool is_work);

	
	
	~Logger();
private:
	
	class Postgre* connection;
	pqxx::connection* conn;
	
};