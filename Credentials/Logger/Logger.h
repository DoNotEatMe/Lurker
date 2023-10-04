#pragma once


namespace pqxx {
	class connection;
}

class Logger {
public:
	Logger();

	void post(std::string programm_name, std::string message, std::string type);
	void status(std::string programm_name, bool is_work);

	
	
	~Logger();
private:
	
	pqxx::connection* conn;
	
};