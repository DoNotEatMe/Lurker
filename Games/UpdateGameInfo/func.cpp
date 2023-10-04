#include <func.h>
#include <iostream>
#include <Logger/Logger.h>
#include <cURLing.h>
#include <pqxx/pqxx>
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <Postgre.h>
#include <chrono>
#include <regex>

std::string UpdateGameInfo::reformatDate(const std::string& dateStr) {
	// Regular expressions for matching date patterns
	std::regex monthDayYearRegex(R"((\w{3} \d{1,2}, \d{4}))");
	std::regex monthYearRegex(R"((\w{3} \d{4}))");

	std::smatch match;

	// Check if the input string matches the "Month Day, Year" pattern
	if (std::regex_match(dateStr, match, monthDayYearRegex)) {
		return match[0];
	}

	// Check if the input string matches the "Month Year" pattern
	if (std::regex_match(dateStr, match, monthYearRegex)) {
		return match[0];
	}

	// Handle non-date strings here, return an empty string
	return "";
}


void UpdateGameInfo::mainFunc() {

	Logger log;

	bool test = 1;
	log.status("UpdateGameInfo", test);

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

	std::vector <std::string> dates;
	dates.push_back("Coming soon");
	dates.push_back("Q4 2023");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2024");
	dates.push_back("To be announced");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2023");
	dates.push_back("To be announced");
	dates.push_back("2023");
	dates.push_back("2024");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("2024");
	dates.push_back("To be announced");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("To be announced");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("2024");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("December 2023");
	dates.push_back("Coming soon");
	dates.push_back("Q1 2025");
	dates.push_back("To be announced");
	dates.push_back("To be announced");
	dates.push_back("2023");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("To be announced");
	dates.push_back("October 2023");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2025");
	dates.push_back("2024");
	dates.push_back("Coming soon");
	dates.push_back("Q2 2024");
	dates.push_back("Coming soon");
	dates.push_back("October 2023");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("2024");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2023");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2023");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2023");
	dates.push_back("To be announced");
	dates.push_back("2024");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("2024");
	dates.push_back("2023");
	dates.push_back("Q4 2023");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Q1 2024");
	dates.push_back("2024");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Q4 2023");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("May 18, 2023");
	dates.push_back("Jul 31, 2023");
	dates.push_back("Jun 17, 2023");
	dates.push_back("May 15, 2023");
	dates.push_back("To be announced");
	dates.push_back("May 3, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("Apr 12, 2023");
	dates.push_back("May 18, 2023");
	dates.push_back("Apr 14, 2023");
	dates.push_back("Apr 11, 2023");
	dates.push_back("Aug 19, 2023");
	dates.push_back("May 8, 2023");
	dates.push_back("Jun 26, 2023");
	dates.push_back("May 22, 2023");
	dates.push_back("Apr 29, 2023");
	dates.push_back("Apr 28, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Apr 13, 2023");
	dates.push_back("Sep 13, 2023");
	dates.push_back("2023");
	dates.push_back("Apr 13, 2023");
	dates.push_back("Apr 12, 2023");
	dates.push_back("May 11, 2023");
	dates.push_back("Apr 12, 2023");
	dates.push_back("Apr 12, 2023");
	dates.push_back("Coming soon");
	dates.push_back("May 7, 2023");
	dates.push_back("Apr 12, 2023");
	dates.push_back("Apr 12, 2023");
	dates.push_back("Q1 2024");
	dates.push_back("To be announced");
	dates.push_back("Apr 12, 2023");
	dates.push_back("Q4 2023");
	dates.push_back("Coming soon");
	dates.push_back("Jun 21, 2023");
	dates.push_back("May 22, 2023");
	dates.push_back("May 5, 2023");
	dates.push_back("May 21, 2023");
	dates.push_back("Jul 26, 2023");
	dates.push_back("To be announced");
	dates.push_back("May 17, 2023");
	dates.push_back("To be announced");
	dates.push_back("October 2023");
	dates.push_back("To be announced");
	dates.push_back("May 25, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("May 2, 2023");
	dates.push_back("Apr 24, 2023");
	dates.push_back("To be announced");
	dates.push_back("Apr 21, 2023");
	dates.push_back("Apr 11, 2023");
	dates.push_back("May 24, 2023");
	dates.push_back("Jul 28, 2023");
	dates.push_back("Apr 28, 2023");
	dates.push_back("May 30, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Aug 22, 2023");
	dates.push_back("Jun 20, 2023");
	dates.push_back("2024");
	dates.push_back("Apr 27, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("May 17, 2023");
	dates.push_back("May 17, 2023");
	dates.push_back("May 12, 2023");
	dates.push_back("Apr 11, 2023");
	dates.push_back("Aug 3, 2023");
	dates.push_back("Jun 2, 2023");
	dates.push_back("May 5, 2023");
	dates.push_back("Jun 22, 2023");
	dates.push_back("Apr 19, 2023");
	dates.push_back("May 5, 2023");
	dates.push_back("Jun 15, 2023");
	dates.push_back("Aug 17, 2023");
	dates.push_back("Apr 21, 2023");
	dates.push_back("May 2, 2023");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("May 17, 2023");
	dates.push_back("Aug 14, 2023");
	dates.push_back("Apr 15, 2023");
	dates.push_back("Apr 15, 2023");
	dates.push_back("Q4 2023");
	dates.push_back("Aug 29, 2023");
	dates.push_back("To be announced");
	dates.push_back("May 3, 2023");
	dates.push_back("May 2, 2023");
	dates.push_back("To be announced");
	dates.push_back("Sep 6, 2023");
	dates.push_back("Apr 28, 2023");
	dates.push_back("Apr 18, 2023");
	dates.push_back("Jun 1, 2023");
	dates.push_back("2024");
	dates.push_back("Apr 12, 2023");
	dates.push_back("May 2, 2023");
	dates.push_back("To be announced");
	dates.push_back("Apr 25, 2023");
	dates.push_back("Jun 7, 2023");
	dates.push_back("May 9, 2023");
	dates.push_back("Apr 10, 2023");
	dates.push_back("Apr 25, 2023");
	dates.push_back("Apr 25, 2023");
	dates.push_back("Jun 1, 2023");
	dates.push_back("May 29, 2023");
	dates.push_back("To be announced");
	dates.push_back("2024");
	dates.push_back("Apr 19, 2023");
	dates.push_back("Apr 22, 2023");
	dates.push_back("Apr 28, 2023");
	dates.push_back("May 7, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Apr 27, 2023");
	dates.push_back("Jun 28, 2023");
	dates.push_back("May 2, 2023");
	dates.push_back("Coming soon");
	dates.push_back("May 4, 2023");
	dates.push_back("May 4, 2023");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("Apr 12, 2023");
	dates.push_back("May 8, 2023");
	dates.push_back("Jun 30, 2023");
	dates.push_back("Jul 14, 2023");
	dates.push_back("Apr 29, 2023");
	dates.push_back("May 12, 2023");
	dates.push_back("Jul 14, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("To be announced");
	dates.push_back("May 27, 2023");
	dates.push_back("2023");
	dates.push_back("May 17, 2023");
	dates.push_back("May 12, 2023");
	dates.push_back("Apr 11, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Aug 2, 2023");
	dates.push_back("Apr 25, 2023");
	dates.push_back("Apr 11, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Apr 16, 2023");
	dates.push_back("May 3, 2023");
	dates.push_back("Apr 11, 2023");
	dates.push_back("To be announced");
	dates.push_back("Apr 29, 2023");
	dates.push_back("Coming soon");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("Apr 24, 2023");
	dates.push_back("Apr 10, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Apr 11, 2023");
	dates.push_back("Coming soon");
	dates.push_back("Coming soon");
	dates.push_back("Apr 20, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("Apr 27, 2023");
	dates.push_back("May 21, 2023");
	dates.push_back("May 5, 2023");
	dates.push_back("To be announced");
	dates.push_back("Coming soon");
	dates.push_back("May 5, 2023");
	dates.push_back("Q4 2023");
	dates.push_back("May 1, 2023");

	for (auto& a : dates) {

		std::string res = reformatDate(a);

		if (!res.empty()) {
			std::cout << "ok: " << a << " " << res << std::endl;
		}
		else {
			std::cout << "not: " << a << " " << res << std::endl;
		}

	}

	/*date data set

	int count = 0;
	if (count < 100) {
		for (auto& appid : UpdateAppid){

			std::string link = "https://store.steampowered.com/api/appdetails?appids=" + std::to_string(appid) + "&&cc=US";
			cURLing curl;
			curl.getHTML(link.c_str());


			rapidjson::Document document;
			document.Parse(curl.GetBuffer().c_str());
			curl.clearBuffer();

			if (document.IsObject() && document[std::to_string(appid).c_str()]["success"].GetBool() == true && document[std::to_string(appid).c_str()].IsObject()) {
				rapidjson::Value& app = document[std::to_string(appid).c_str()]["data"];
				if (app.HasMember("release_date") && app["release_date"].HasMember("date") && !app["release_date"]["date"].IsNull()) {
					std::cout << "dates.push_back('" << app["release_date"]["date"].GetString() << "');" << std::endl;
					count++;
				}
			}
		}
	}
	//end date data set
	*/

	/*
	for (auto& appid : UpdateAppid) {
		std::cout << "current appid: " << appid << std::endl;
		auto start_time = std::chrono::high_resolution_clock::now();

		//???
		auto currentTime = std::chrono::system_clock::now();
		std::time_t timestamp = std::chrono::system_clock::to_time_t(currentTime);

		try {

			appid = 2397510;
			std::string link = "https://store.steampowered.com/api/appdetails?appids=" + std::to_string(appid) + "&&cc=US";
			//std::string link = "https://store.steampowered.com/api/appdetails?appids=594650&&cc=US";

			cURLing curl;
			curl.getHTML(link.c_str());

			rapidjson::Document document;
			document.Parse(curl.GetBuffer().c_str());
			curl.clearBuffer();


			if (document.IsObject() && document[std::to_string(appid).c_str()]["success"].GetBool() == true && document[std::to_string(appid).c_str()].IsObject() ) {
				rapidjson::Value& app = document[std::to_string(appid).c_str()]["data"];

				// Games

				pqxx::params game_type;
				pqxx::params game_is_free;
				pqxx::params game_short_description;
				pqxx::params game_long_description;
				pqxx::params game_website;
				pqxx::params game_initial_price;
				pqxx::params game_platform_windows;
				pqxx::params game_platform_mac;
				pqxx::params game_platform_linux;
				pqxx::params game_metacritic_score;
				pqxx::params game_total_recommendations;
				pqxx::params game_coming_soon;
				pqxx::params game_release_date;
				pqxx::params game_support_url;
				pqxx::params game_support_mail;

				if (app.HasMember("type") && !app["type"].IsNull()) {
					game_type.append(app["type"].GetString());
				}
				else { game_type.append(); }

				if (app.HasMember("is_free") && !app["is_free"].IsNull()) {
					game_is_free.append(app["is_free"].GetBool());
				}
				else { game_is_free.append(); }

				if (app.HasMember("short_description") && !app["short_description"].IsNull()) {
					game_short_description.append(app["short_description"].GetString());
				}
				else { game_short_description.append(); }

				if (app.HasMember("detailed_description") && !app["detailed_description"].IsNull()) {
					game_long_description.append(app["detailed_description"].GetString());
				}
				else { game_long_description.append(); }

				if (app.HasMember("website") && !app["website"].IsNull()) {
					game_website.append(app["website"].GetString());
				}
				else { game_website.append(); }

				// TODO: Make correction to price (/100) && field type in DB
				if (app.HasMember("price_overview") && app["price_overview"].HasMember("initial") && !app["price_overview"]["initial"].IsNull()) {
					game_initial_price.append(app["price_overview"]["initial"].GetInt());
				}
				else { game_initial_price.append(); }

				if (app.HasMember("platforms") && app["platforms"].HasMember("windows") && !app["platforms"]["windows"].IsNull()) {
					game_platform_windows.append(app["platforms"]["windows"].GetBool());
				}
				else { game_platform_windows.append(); }

				if (app.HasMember("platforms") && app["platforms"].HasMember("mac") && !app["platforms"]["mac"].IsNull()) {
					game_platform_mac.append(app["platforms"]["mac"].GetBool());
				}
				else { game_platform_mac.append(); }

				if (app.HasMember("platforms") && app["platforms"].HasMember("linux") && !app["platforms"]["linux"].IsNull()) {
					game_platform_linux.append(app["platforms"]["linux"].GetBool());
				}
				else { game_platform_linux.append(); }

				if (app.HasMember("metacritic") && app["metacritic"].HasMember("score") && !app["metacritic"]["score"].IsNull()) {
					game_metacritic_score.append(app["metacritic"]["score"].GetInt());
				}
				else { game_metacritic_score.append(); }

				// TODO: Check what is recommendations exactly is?
				if (app.HasMember("recommendations") && app["recommendations"].HasMember("total") && !app["recommendations"]["total"].IsNull()) {
					game_total_recommendations.append(app["recommendations"]["total"].GetInt());
				}
				else { game_total_recommendations.append(); }

				if (app.HasMember("release_date") && app["release_date"].HasMember("coming_soon") && !app["release_date"]["coming_soon"].IsNull()) {
					game_coming_soon.append(app["release_date"]["coming_soon"].GetBool());
				}
				else { game_coming_soon.append(); }

				// TODO: Date format to timestamp && DB field to timestamp  || Coming soon, Q4,
				if (app.HasMember("release_date") && app["release_date"].HasMember("date") && !app["release_date"]["date"].IsNull()) {
					game_release_date.append(app["release_date"]["date"].GetString());

					pqxx::params params;
					params.append(app["release_date"]["date"].GetString()); // Assuming you're adding a date as a string

					std::vector <std::string> dates;
					dates.push_back("Q4");
					dates.push_back("Coming soon");
					dates.push_back("Q4 2023");
					dates.push_back("2023");
					dates.push_back("To be announced");
					dates.push_back("Mar 19 2023");
					dates.push_back("Mar, 19, 2023");
					dates.push_back("Mar ,19 2023");
					dates.push_back("Mar Junasd 19 2023");
					"Apr 21, 2023"
						"Jul 25, 2023"











				}
				else { game_release_date.append(); }

				if (app.HasMember("support_info") && app["support_info"].HasMember("url") && !app["support_info"]["url"].IsNull()) {
					game_support_url.append(app["support_info"]["url"].GetString());
				}
				else { game_support_url.append(); }

				if (app.HasMember("support_info") && app["support_info"].HasMember("email") && !app["support_info"]["email"].IsNull()) {
					game_support_mail.append(app["support_info"]["email"].GetString());
				}
				else { game_support_mail.append(); }

				std::string games = "UPDATE games SET game_type = $1, game_is_free = $2, game_short_description = $3, game_long_description = $4, game_website = $5, game_initial_price = $6, game_platform_windows = $7, game_platform_mac = $8, game_platform_linux = $9, game_metacritic_score = $10, game_total_recommendations = $11, game_coming_soon = $12, game_release_date = $13, game_support_url = $14, game_support_mail = $15, game_last_db_update = NOW() WHERE pk_game_appid = $16;";

				try {
					txn.exec_params(games,
						game_type,
						game_is_free,
						game_short_description,
						game_long_description,
						game_website,
						game_initial_price,
						game_platform_windows,
						game_platform_mac,
						game_platform_linux,
						game_metacritic_score,
						game_total_recommendations,
						game_coming_soon,
						game_release_date,
						game_support_url,
						game_support_mail,
						appid);

				}
				catch (const std::exception& e) {
					std::cerr << "Update all games | games error: " << e.what() << std::endl;
				}



				// Publishers
				if (app.HasMember("publishers")) {
					rapidjson::Value& pubArr = app["publishers"];
					for (rapidjson::SizeType i = 0; i < pubArr.Size(); ++i) {
						pqxx::params publisher_name;
						publisher_name.append(pubArr[i].GetString());

						std::string publisherOne = "INSERT INTO publishers (publisher_name) VALUES ($1) ON CONFLICT (publisher_name) DO NOTHING;";
						std::string publisherTwo = "INSERT INTO game_publisher (fk_game_appid, fk_publisher_id) SELECT $1, pk_publisher_id FROM publishers WHERE publisher_name = $2;";
						try {
							txn.exec_params(publisherOne,
								publisher_name
								);
							txn.exec_params(publisherTwo,
								appid,
								publisher_name
								);
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
						pqxx::params developer_name;
						developer_name.append(devArr[i].GetString());

						std::string developerOne = "INSERT INTO developers (developer_name) VALUES ($1) ON CONFLICT (developer_name) DO NOTHING;";
						std::string developerTwo = "INSERT INTO game_developer (fk_game_appid, fk_developer_id) SELECT $1, pk_developer_id FROM developers WHERE developer_name = $2;";
						try {
							txn.exec_params(developerOne,
								developer_name
								);
							txn.exec_params(developerTwo,
								appid,
								developer_name
							);
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
						pqxx::params genre_id;
						pqxx::params genre_name;
						genre_id.append(genArr[i]["id"].GetString());
						genre_name.append(genArr[i]["description"].GetString());

						std::string genresOne = "INSERT INTO genres (pk_genre_id, genre_name) VALUES ($1, $2) ON CONFLICT (pk_genre_id, genre_name) DO NOTHING; ";
						std::string genresTwo = "INSERT INTO game_genres(fk_game_appid, fk_genre_id) SELECT $1, pk_genre_id FROM genres WHERE genre_name = $2 ON CONFLICT(fk_game_appid, fk_genre_id) DO NOTHING; ";
						try {
							txn.exec_params(genresOne,
								genre_id,
								genre_name
								);
							txn.exec_params(genresTwo,
								appid,
								genre_name
							);
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
						pqxx::params category_id;
						pqxx::params category_name;
						category_id.append(catArr[i]["id"].GetInt());
						category_name.append(catArr[i]["description"].GetString());

						std::string categoriesOne = "INSERT INTO categories (pk_category_id, category_name) VALUES ($1, $2) ON CONFLICT (pk_category_id, category_name) DO NOTHING;";
						std::string categoriesTwo = "INSERT INTO game_category (fk_game_appid, fk_category_id) SELECT $1, pk_category_id FROM categories WHERE category_name = $2 ON CONFLICT (fk_game_appid, fk_category_id) DO NOTHING;";
						try {
							txn.exec_params(categoriesOne,
								category_id,
								category_name
								);
							txn.exec_params(categoriesTwo,
								appid,
								category_name
								);
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
						pqxx::params dlc_id;
						dlc_id.append(dlcArr[i].GetInt());

						std::string dlc = "INSERT INTO game_dlc (fk_game_appid, dlc_id) VALUES ($1,$2);";

						try {
							txn.exec_params(dlc,
								appid,
								dlc_id
								);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | dlc error: " << e.what() << std::endl;
						}
					}
				}

				// DLC's from type "!game"
				if (app.HasMember("type") && !app["type"].IsNull() && app["type"].GetString() != "game" ) {

					if (app.HasMember("fullgame") && app["fullgame"].HasMember("appid") && !app["fullgame"]["appid"].IsNull()) {
						pqxx::params fullgame_id;
						fullgame_id.append(app["fullgame"]["appid"].GetString());
						pqxx::params dlc_type;
						dlc_type.append(app["type"].GetString());

						std::string dlc_notgame = "INSERT INTO game_dlc (fk_game_appid, dlc_id, dlc_type) VALUES ($1,$2, $3) ON CONFLICT (fk_game_appid, dlc_id) DO UPDATE SET fk_game_appid = EXCLUDED.fk_game_appid, dlc_id = EXCLUDED.dlc_id, dlc_type = EXCLUDED.dlc_type;";

						try{
							txn.exec_params(dlc_notgame,
								fullgame_id,
								appid,
								dlc_type
							);
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

						pqxx::params lang_name;
						lang_name.append(match.str());

						std::string language = "INSERT INTO game_language (fk_game_appid, language) VALUES ($1,$2) ON CONFLICT DO NOTHING;";

						try{
							txn.exec_params(language,
								appid,
								lang_name
							);
						}
						catch (const std::exception& e) {
							std::cerr << "Update all games | language error: " << e.what() << std::endl;
						}

						++it;
					}
				}

				txn.commit();

			}

			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

			std::cout << appid << " done" << std::endl;
			std::cout << "Query execution time: " << duration.count() << " ms" << std::endl;
			std::cout << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Update all games | Main loop error: " << e.what() << std::endl;
		}


	}
	*/
}


