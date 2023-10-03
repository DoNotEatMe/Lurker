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
	//DB.TablesCheck();

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
		pqxx::work txn(*DB.Connect());
		try {
			//appid = 594650;
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

				
				// Games
				std::string games = "UPDATE games SET ";
				games += "game_type = ";
				if (app.HasMember("type") && !app["type"].IsNull()) {
					games += "'";
					games += app["type"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_is_free = ";
				if (app.HasMember("is_free") && !app["is_free"].IsNull()) {
					games += "'";
					games += std::to_string(app["is_free"].GetBool());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_short_description = ";
				if (app.HasMember("short_description") && !app["short_description"].IsNull()) {
					games += "'";
					games += app["short_description"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_long_description = ";
				if (app.HasMember("detailed_description") && !app["detailed_description"].IsNull()) {
					games += "'";
					games += app["detailed_description"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_website = ";
				if (app.HasMember("website") && !app["website"].IsNull()) {
					games += "'";
					games += app["website"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				// TODO: Make correction to price (/100) && value type in DB
				games += "game_initial_price = ";
				if (app.HasMember("price_overview") && app["price_overview"].HasMember("initial") && !app["price_overview"]["initial"].IsNull()) {
					games += "'";
					games += std::to_string(app["price_overview"]["initial"].GetInt());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_platform_windows = ";
				if (app.HasMember("platforms") && app["platforms"].HasMember("windows") && !app["platforms"]["windows"].IsNull()) {
					games += "'";
					games += std::to_string(app["platforms"]["windows"].GetBool());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_platform_mac = ";
				if (app.HasMember("platforms") && app["platforms"].HasMember("mac") && !app["platforms"]["mac"].IsNull()) {
					games += "'";
					games += std::to_string(app["platforms"]["mac"].GetBool());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_platform_linux = ";
				if (app.HasMember("platforms") && app["platforms"].HasMember("linux") && !app["platforms"]["linux"].IsNull()) {
					games += "'";
					games += std::to_string(app["platforms"]["linux"].GetBool());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_metacritic_score = ";
				if (app.HasMember("metacritic") && app["metacritic"].HasMember("score") && !app["metacritic"]["score"].IsNull()) {
					games += "'";
					games += std::to_string(app["metacritic"]["score"].GetInt());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_total_recommendations = ";
				if (app.HasMember("recommendations") && app["recommendations"].HasMember("total") && !app["recommendations"]["total"].IsNull()) {
					games += "'";
					games += std::to_string(app["recommendations"]["total"].GetInt());
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_coming_soon = ";
				if (app.HasMember("release_date") && app["release_date"].HasMember("coming_soon") && !app["release_date"]["coming_soon"].IsNull()) {
					games += "'";
					games += std::to_string(app["release_date"]["coming_soon"].GetBool());
					games += "', ";
				}
				else { games += "NULL, "; }

				// TODO: Date format to timestamp
				games += "game_release_date = ";
				if (app.HasMember("release_date") && app["release_date"].HasMember("date") && !app["release_date"]["date"].IsNull()) {
					games += "'";
					games += app["release_date"]["date"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_support_url = ";
				if (app.HasMember("support_info") && app["support_info"].HasMember("url") && !app["support_info"]["url"].IsNull()) {
					games += "'";
					games += app["support_info"]["url"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_support_mail = ";
				if (app.HasMember("support_info") && app["support_info"].HasMember("email") && !app["support_info"]["email"].IsNull()) {
					games += "'";
					games += app["support_info"]["email"].GetString();
					games += "', ";
				}
				else { games += "NULL, "; }

				games += "game_last_db_update = NOW()";

				auto currentTime = std::chrono::system_clock::now();
				std::time_t timestamp = std::chrono::system_clock::to_time_t(currentTime);

				games += " WHERE pk_game_appid = " + std::to_string(appid) + ";";

				try {
					txn.exec(games);
				}
				catch (const std::exception& e) {
					std::cerr << "Update all games | games error: " << e.what() << std::endl;
				}



				// Publishers
				if (app.HasMember("publishers")) {
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

						try {
							txn.exec(publisher);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | publishers error: " << e.what() << std::endl;
						}
					}
				}

				// Developers
				if (app.HasMember("developers")) {
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

						try {
							txn.exec(developer);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | developers error: " << e.what() << std::endl;
						}
					}
				}

				// Genres
				if (app.HasMember("genres")) {
					rapidjson::Value& genArr = app["genres"];
					for (rapidjson::SizeType i = 0; i < genArr.Size(); i++) {
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

						try {
							txn.exec(genres);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | genres error: " << e.what() << std::endl;
						}
					}
				}

				// Categories
				if (app.HasMember("categories")) {
					rapidjson::Value& catArr = app["categories"];
					for (rapidjson::SizeType i = 0; i < catArr.Size(); ++i) {
						std::string categories = "INSERT INTO categories (pk_category_id, category_name) VALUES ('";
						categories += std::to_string(catArr[i]["id"].GetInt());
						categories += "', '";
						categories += catArr[i]["description"].GetString();
						categories += "') ON CONFLICT (pk_category_id, category_name) DO NOTHING;";
						categories += "INSERT INTO game_category (fk_game_appid, fk_category_id) SELECT '";
						categories += std::to_string(appid);
						categories += "', pk_category_id FROM categories WHERE category_name = '";
						categories += catArr[i]["description"].GetString();
						categories += "' ON CONFLICT (fk_game_appid, fk_category_id) DO NOTHING;";

						try {
							txn.exec(categories);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | categories error: " << e.what() << std::endl;
						}

					}
				}

				// DLC's from type "game"
				if (app.HasMember("dlc")) {
					rapidjson::Value& dlcArr = app["dlc"];
					for (rapidjson::SizeType i = 0; i < dlcArr.Size(); ++i) {
						std::string dlc = "INSERT INTO game_dlc (fk_game_appid, dlc_id) VALUES ('";
						dlc += std::to_string(appid);
						dlc += "', '";
						dlc += std::to_string(dlcArr[i].GetInt());
						dlc += "');";

						try {
							txn.exec(dlc);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | dlc error: " << e.what() << std::endl;
						}
					}
				}

				// DLC's from type "!game"
				if (app.HasMember("type") && !app["type"].IsNull() && app["type"].GetString() != "game" ) {

					if (app.HasMember("fullgame") && app["fullgame"].HasMember("appid") && !app["fullgame"]["appid"].IsNull()) {
						std::string dlctype = "INSERT INTO game_dlc (fk_game_appid, dlc_id, dlc_type) VALUES ('";
						dlctype += app["fullgame"]["appid"].GetString();
						dlctype += "', '";
						dlctype += std::to_string(appid);
						dlctype += "', '";
						dlctype += app["type"].GetString();
						dlctype += "') ON CONFLICT (fk_game_appid, dlc_id) DO UPDATE SET fk_game_appid = EXCLUDED.fk_game_appid, dlc_id = EXCLUDED.dlc_id, dlc_type = EXCLUDED.dlc_type;";

						try{
							txn.exec(dlctype);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | dlc (dlc_type) error: " << e.what() << std::endl;
						}
					}
				}

				// Languages
				if (app.HasMember("supported_languages") && !app["supported_languages"].IsNull()){
					std::string input = app["supported_languages"].GetString();

					// Regular expression pattern to match languages
					std::regex pattern("([A-Z][a-z]+(?:[ -][A-Z][a-z]+)*)");

					// Iterate through matches
					std::sregex_iterator it(input.begin(), input.end(), pattern);
					std::sregex_iterator end;

					
					while (it != end) {
						std::smatch match = *it;
						std::string language = "INSERT INTO game_language (fk_game_appid, language) VALUES ('";
						language += std::to_string(appid);
						language += "', '";
						language += match.str();
						language += "') ON CONFLICT DO NOTHING;";

						try{
							txn.exec(language);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | language error: " << e.what() << std::endl;
						}

						++it;
					}
				}

				txn.commit();
				pqxx::work txn(*DB.Connect());
				std::cout << appid << " done" << std::endl;

			}




			

			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			std::cout << std::endl;
			std::cout << "Query execution time: " << duration.count() << " ms" << std::endl;

		}
		catch (const std::exception& e) {
			std::cerr << "Update all games | Main loop error: " << e.what() << std::endl;
		}


	}
	
}





