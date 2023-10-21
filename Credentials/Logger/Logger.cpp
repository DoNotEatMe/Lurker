//internal 
#include <Logger.h>
#include <Postgre.h>
//system
#include <iostream>
//vcpkg
#include <pqxx/pqxx>



Logger::Logger(pqxx::connection* db)
{
    DBCon = db;
}

void Logger::post(std::string programm_name, int appid, std::string message, std::string type)
{
    try{
        std::string request = "INSERT INTO logger (programm_name, game_appid, message,type,time) VALUES ($1,$2,$3,$4,NOW());";
        
        pqxx::work txn(*DBCon);
        txn.exec("SET application_name = 'Logger post'");

        txn.exec_params(request,
            programm_name,
            appid,
            message,
            type
        );
        txn.commit();
   
    }
    catch (const std::exception& e) {
        std::cerr << "Logger | post error: " << e.what() << std::endl;
    }
}

void Logger::status(std::string programm_name, bool is_work)
{
    try{
        pqxx::work txn(*DBCon);

        std::string request = "INSERT INTO status (programm_name,is_work,time) VALUES ($1,$2,NOW()) ON CONFLICT (programm_name) DO UPDATE SET programm_name = EXCLUDED.programm_name, is_work = EXCLUDED.is_work, time = NOW();";
        txn.exec("SET application_name = 'Logger status'");

        txn.exec_params(request,
            programm_name,
            is_work);
        txn.commit();


    }
  catch (const std::exception& e) {
        std::cerr << "Logger | status error: " << e.what() << std::endl;
    }
}

Logger::~Logger()
{
}
