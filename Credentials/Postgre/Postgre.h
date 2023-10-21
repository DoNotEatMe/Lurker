#pragma once

namespace pqxx {
	class connection;
}


class Postgre {

public:
	Postgre();
	pqxx::connection* Connect();
	pqxx::connection* Connection;
	void TablesCheck();
	

	~Postgre();

private:
	


};