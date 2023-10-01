#include <iostream>
#include "InitAllGames.h"
#include <cURLing.h>

#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <Postgre.h>

#include <chrono>


/*
*	Execution time ~50s
*	176220 rows of appid and game titles
*/



int main() {
	Postgre DB;
	DB.TablesCheck();
		
	std::string link = "http://api.steampowered.com/ISteamApps/GetAppList/v0002/?format=json";

	cURLing curl;
	curl.getHTML(link.c_str());
	
	rapidjson::Document document;
	document.Parse(curl.GetBuffer().c_str());

	const rapidjson::Value& apps = document["applist"]["apps"];
	
	if (apps.IsArray()) {
		
		pqxx::connection* connPtr = DB.Connect();
		
		try {
			pqxx::work txn(*connPtr);
			
			std::string sql = "INSERT INTO games (pk_game_appid, game_name) VALUES ($1, $2) ON CONFLICT (pk_game_appid) DO NOTHING";
						
			auto start_time = std::chrono::high_resolution_clock::now();
					
			const size_t batchSize = 100;
			
			for (rapidjson::SizeType i = 0; i < apps.Size(); ++i) {
				const rapidjson::Value& game = apps[i];
				if (game.IsObject() && game.HasMember("appid") && game.HasMember("name")) {
										
					
					pqxx::params params;
					params.append(game["appid"].GetInt());
					params.append(game["name"].GetString());

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
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
}