//internal 
#include <cURLing.h>
#include <Postgre.h>
#include <Logger/Logger.h>

//system
#include <iostream>
#include <chrono>
//vcpkg
#include <pqxx/pqxx>
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


/*
*	Execution time ~50s
*	176220 rows of appid and game titles
*/


int main() {
	Postgre DB;
	pqxx::connection* connPtr = DB.Connect();
	DB.TablesCheck();
		
	std::string link = "http://api.steampowered.com/ISteamApps/GetAppList/v0002/?format=json";

	Logger log(connPtr);
	cURLing curl(&log);

	curl.getHTML(link.c_str());
	
	rapidjson::Document document;
	document.Parse(curl.GetBuffer().c_str());

	curl.clearBuffer();

	const rapidjson::Value& apps = document["applist"]["apps"];

	
	pqxx::work txn(*connPtr);
	txn.exec("SET application_name = 'Logger post'");

	std::string sql = "SELECT COUNT(*) FROM games";
	pqxx::result res = txn.exec(sql);

	if (apps.Size() != std::stoi(res[0][0].c_str())) {
		
		if (apps.IsArray()) {
		
			try {
			
				std::string sql = "INSERT INTO games (pk_game_appid, game_name) VALUES ($1, $2) ON CONFLICT (pk_game_appid) DO NOTHING";
						
				auto start_time = std::chrono::high_resolution_clock::now();
					
				const size_t batchSize = 1000;
			
				for (rapidjson::SizeType i = 0; i < apps.Size(); ++i) {
					const rapidjson::Value& game = apps[i];
					if (game.IsObject() && game.HasMember("appid") && game.HasMember("name")) {
					
						txn.exec_params(sql, game["appid"].GetInt(), game["name"].GetString());
					
						if (i > 0 && i % batchSize == 0) {
							txn.commit();
							pqxx::work txn(*connPtr);
						}
					
					}
				}

				txn.commit();
						
				auto end_time = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
						
				std::cout << "Query execution time: " << duration.count() << " seconds" << std::endl;

			} catch (const std::exception& e) {
				std::string error = e.what();
				log.post("InitAllAGames", 0, error, "warning");
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
	}
}