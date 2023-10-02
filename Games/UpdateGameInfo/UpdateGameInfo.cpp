#include <iostream>
#include "UpdateGameInfo.h"
#include <cURLing.h>

#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <Postgre.h>

#include <chrono>
#include <regex>


int main() {

	/*---------------------------------------------------------*/
	/*  get all appid from DB, where game_last_update is empty */
	/*  store appids in vector                                 */	
	/*---------------------------------------------------------*/

	Postgre DB;
	DB.TablesCheck();

	pqxx::work txn(*DB.Connect());
	std::string sql = "SELECT pk_game_appid FROM games WHERE game_last_db_update IS NULL";
	pqxx::result res = txn.exec(sql);

	std::vector<int> UpdateAppid;

	for (auto& a : res) {
		UpdateAppid.push_back(a["pk_game_appid"].as<int>());
	}
	res.clear();

	/*-------------------------------------------------------*/
	/* main loop forming SQL requests to properly store data */
	/*-------------------------------------------------------*/

	for (auto& appid : UpdateAppid) {

		appid = 594650;
		std::string link = "https://store.steampowered.com/api/appdetails?appids=" + std::to_string(appid) + "&&cc=US";
		//std::string link = "https://store.steampowered.com/api/appdetails?appids=594650&&cc=US";
		
		auto start_time = std::chrono::high_resolution_clock::now();

		cURLing curl;
		curl.getHTML(link.c_str());

		rapidjson::Document document;
		document.Parse(curl.GetBuffer().c_str());
		curl.clearBuffer();

				
		if (document[std::to_string(appid).c_str()].IsObject() && document[std::to_string(appid).c_str()]["success"].GetBool() == true) {
			rapidjson::Value& app = document[std::to_string(appid).c_str()]["data"];


			
			std::string games = "UPDATE games SET ";

			games += "game_type = ";
			if (app.HasMember("type")) {
				games += "'";
				games += app["type"].GetString();
				games += "', ";
			} 
			else { games += ", "; }

			games += "game_is_free = ";
			if (app.HasMember("is_free")) {
				games += "'";
				games += std::to_string(app["is_free"].GetBool());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_short_description = ";
			if (app.HasMember("short_description")) {
				games += "'";
				games += app["short_description"].GetString();
				games += "', ";
			}
			else { games += ", "; }
			
			games += "game_long_description = ";
			if (app.HasMember("detailed_description")) {
				games += "'";
				games += app["detailed_description"].GetString();
				games += "', ";
			}
			else { games += ", "; }
			
			games += "game_website = "; 
			if (app.HasMember("website")) {
				games += "'";
				games += app["website"].GetString();
				games += "', ";
			}
			else { games += ", "; }

			games += "game_initial_price = ";
			if (app.HasMember("price_overview") && app["price_overview"].HasMember("initial")) {
				games += "'";
				games += std::to_string(app["price_overview"]["initial"].GetInt());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_platform_windows = ";
			if (app.HasMember("platforms") && app["platforms"].HasMember("windows")) {
				games += "'";
				games += std::to_string(app["platforms"]["windows"].GetBool());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_platform_mac = ";
			if (app.HasMember("platforms") && app["platforms"].HasMember("mac")) {
				games += "'";
				games += std::to_string(app["platforms"]["mac"].GetBool());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_platform_linux = ";
			if (app.HasMember("platforms") && app["platforms"].HasMember("linux")) {
				games += "'";
				games += std::to_string(app["platforms"]["linux"].GetBool());
				games += "', ";
			}
			else { games += ", "; }
			
			games += "game_metacritic_score = ";
			if (app.HasMember("metacritic") && app["metacritic"].HasMember("score")) {
				games += "'";
				games += std::to_string(app["metacritic"]["score"].GetInt());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_total_recommendations = ";
			if (app.HasMember("recommendations") && app["recommendations"].HasMember("total")) {
				games += "'";
				games += std::to_string(app["recommendations"]["total"].GetInt());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_coming_soon = ";
			if (app.HasMember("release_date") && app["release_date"].HasMember("coming_soon")) {
				games += "'";
				games += std::to_string(app["release_date"]["coming_soon"].GetBool());
				games += "', ";
			}
			else { games += ", "; }

			games += "game_release_date = ";
			if (app.HasMember("release_date") && app["release_date"].HasMember("date")) {
				games += "'";
				games += app["release_date"]["date"].GetString();
				games += "', ";
			}
			else { games += ", "; }

			games += "game_support_url = ";
			if (app.HasMember("support_info") && app["support_info"].HasMember("url")) {
				games += "'";
				games += app["support_info"]["url"].GetString();
				games += "', ";
			}
			else { games += ", "; }

			games += "game_support_mail = ";
			if (app.HasMember("support_info") && app["support_info"].HasMember("email")) {
				games += "'";
				games += app["support_info"]["email"].GetString();
				games += "', ";
			}
			else { games += ")"; }

			games += "game_last_db_update = NOW()";

			auto currentTime = std::chrono::system_clock::now();
			std::time_t timestamp = std::chrono::system_clock::to_time_t(currentTime);

			games += " WHERE pk_game_appid = " + std::to_string(appid);

			txn.exec(games);

			

			//Publishers
			rapidjson::Value& pubArr = app["publishers"];
			for (rapidjson::SizeType i = 0; i < pubArr.Size(); ++i) {
				std::string publisher = "INSERT INTO publishers (publisher_name) VALUES ('";
				publisher += pubArr[i].GetString();
				publisher += "') ON CONFLICT (publisher_name) DO NOTHING;";
				publisher += "INSERT INTO game_publisher (fk_game_appid, fk_publisher_id) SELECT '";
				publisher += std::to_string(appid);
				publisher += "', pk_publisher_id FROM publishers WHERE publisher_name = '";
				publisher += pubArr[i].GetString();
				publisher += "';";
				
				std::cout << publisher << std::endl;

				txn.exec(publisher);
			}

			//Developers
			rapidjson::Value& devArr = app["developers"];
			for (rapidjson::SizeType i = 0; i < devArr.Size(); ++i) {
				std::string developer = "INSERT INTO developers (developer_name) VALUES ('";
				developer += devArr[i].GetString();
				developer += "') ON CONFLICT (developer_name) DO NOTHING;";
				developer += "INSERT INTO game_developer (fk_game_appid, fk_developer_id) SELECT '";
				developer += std::to_string(appid);
				developer += "', pk_developer_id FROM developers WHERE developer_name = '";
				developer += devArr[i].GetString();
				developer += "'ON CONFLICT (fk_game_appid, fk_developer_id) DO NOTHING;";
				std::cout << developer << std::endl;

				txn.exec(developer);
			}

			//Genres
			rapidjson::Value& genArr = app["genres"];
			for (rapidjson::SizeType i = 0; i < genArr.Size(); i++) {
				std::cout << "id: " << genArr[i]["id"].GetString() << " desc: " << genArr[i]["description"].GetString() << std::endl;
				std::string genres = "INSERT INTO genres (pk_genre_id, genre_name) VALUES ('";
				genres += genArr[i]["id"].GetString();
				genres += "', '";
				genres += genArr[i]["description"].GetString();
				genres += "') ON CONFLICT (pk_genre_id, genre_name) DO NOTHING;";
				genres += "INSERT INTO game_genres (fk_game_appid, fk_genre_id) SELECT '";
				genres += std::to_string(appid);
				genres += "', pk_genre_id FROM genres WHERE genre_name = '";
				genres += genArr[i]["description"].GetString();
				genres += "' ON CONFLICT (fk_game_appid, fk_genre_id) DO NOTHING;";

				std::cout << genres << std::endl;

				try{
				txn.exec(genres);
				}
				catch (const std::exception& e) {
					std::cerr << "Error: " << e.what() << std::endl;

				}
			}



		}
						
		
		std::string categories = "";
		std::string dlcs = "";
		std::string languages = "";

		

			

		/*

			//TODO: Categories
			std::cout << std::endl;
			std::cout << "categories: " << std::endl << std::endl;
			rapidjson::Value& catArr = app["categories"];
			for (rapidjson::SizeType i = 0; i < catArr.Size(); ++i) {
				std::cout << "id: " << catArr[i]["id"].GetInt() << " desc: " << catArr[i]["description"].GetString() << std::endl;
			}

			// TODO: DLC's
			std::cout << std::endl;
			std::cout << "dlc array: " << std::endl << std::endl;
			rapidjson::Value& dlcArr = app["dlc"];
			for (rapidjson::SizeType i = 0; i < dlcArr.Size(); ++i) {
				if (dlcArr[i].IsInt()) {
					std::cout << dlcArr[i].GetInt() << std::endl;
				}
				else {
					std::cout << "dlc " << i << " is not an int" << std::endl;
				}
			}

			//TODO: Languages
			std::cout << std::endl;
			std::cout << "languages: " << std::endl << std::endl;
			std::string input = app["supported_languages"].GetString();

			// Regular expression pattern to match languages
			std::regex pattern("([A-Z][a-z]+(?:[ -][A-Z][a-z]+)*)");

			// Iterate through matches
			std::sregex_iterator it(input.begin(), input.end(), pattern);
			std::sregex_iterator end;

			while (it != end) {
				std::smatch match = *it;
				std::cout << match.str() << std::endl;
				++it;
			}


*/

			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			std::cout << std::endl;
			std::cout << "Query execution time: " << duration.count() << " ms" << std::endl;

		};
	}



