#pragma once

namespace pqxx {
	class connection;
}


class Postgre {

public:
	Postgre();
	pqxx::connection* Connect();
	void TablesCheck();
	

	~Postgre();

private:
	


};