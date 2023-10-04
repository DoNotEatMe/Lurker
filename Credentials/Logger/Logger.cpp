#include <iostream>
#include <Logger.h>
#include <Postrgre/Postgre.h>
#include <pqxx/pqxx>


Logger::Logger()
{
    Postgre connection;
    conn = connection.Connect();
    
}

//3 strings.
void Logger::post(std::string programm_name, std::string message, std::string type)
{
    //programm_name | now() | error/meggage

    std::string request = "INSERT INTO logger (programm_name,message,type,time) VALUES ($1,$2,$3,NOW());";
    pqxx::work txn(*conn);
    txn.exec_params(request,
        programm_name,
        message,
        type
    );
    txn.commit();
}

void Logger::status(std::string programm_name, bool is_work)
{
    std::string request = "INSERT INTO status (programm_name,is_work,time) VALUES ($1,$2,NOW()) ON CONFLICT (programm_name) DO UPDATE SET programm_name = EXCLUDED.programm_name, is_work = EXCLUDED.is_work, time = EXCLUDED.NOW();";
    pqxx::work txn(*conn);
    txn.exec_params(request,
        programm_name,
        is_work);
    txn.commit();
}





Logger::~Logger()
{
}
