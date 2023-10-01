#pragma once
#include <pqxx/pqxx>


class Postgre {

public:
	Postgre();
	pqxx::connection* Connect();
	void TablesCheck();
	

	~Postgre();

private:
	


};