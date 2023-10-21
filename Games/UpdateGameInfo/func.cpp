//system
#include <iostream>
#include <chrono>
#include <regex>
//internal
#include <func.h>
#include <Logger.h>
#include <cURLing.h>
#include <Postgre.h>
//vcpkg
#include <pqxx/pqxx>
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"



UpdateGameInfo::UpdateGameInfo()
{
	
}

void UpdateGameInfo::mainFunc() {

	Logger log;
	

	/*---------------------------------------------------------*/
	/*  get all appid from DB, where game_last_update is empty */
	/*  store appids in vector                                 */
	/*---------------------------------------------------------*/

	Postgre DB;
	//DB.TablesCheck();

	pqxx::work txn(*DB.Connect());
	
	txn.exec("SET application_name = 'UpdateGameInfo main loop'");

	std::string sql = "SELECT pk_game_appid FROM games WHERE game_last_db_update IS NULL";
	pqxx::result res = txn.exec(sql);

	std::vector<int> UpdateAppid;

	for (auto a : res) {
		UpdateAppid.push_back(a["pk_game_appid"].as<int>());
	}
	res.clear();

	/*-------------------------------------------------------*/
	/* main loop forming SQL requests to properly store data */
	/*-------------------------------------------------------*/
	
	for (auto& appid : UpdateAppid) {
		log.status("UpdateGameInfo", 1);
		auto start_time = std::chrono::high_resolution_clock::now();

		try {

			//appid = 1625260;
			std::string link = "https://store.steampowered.com/api/appdetails?appids=" + std::to_string(appid) + "&&cc=US";

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

				// TODO: check if numeric works well --need updrade cause of /100 dont bring .
				if (app.HasMember("price_overview") && app["price_overview"].HasMember("initial") && !app["price_overview"]["initial"].IsNull()) {
					game_initial_price.append(app["price_overview"]["initial"].GetInt()/100);
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

				// TODO: Check what is recommendations exactly is? not code problem, but actual data
				if (app.HasMember("recommendations") && app["recommendations"].HasMember("total") && !app["recommendations"]["total"].IsNull()) {
					game_total_recommendations.append(app["recommendations"]["total"].GetInt());
				}
				else { game_total_recommendations.append(); }

				if (app.HasMember("release_date") && app["release_date"].HasMember("coming_soon") && !app["release_date"]["coming_soon"].IsNull()) {
					game_coming_soon.append(app["release_date"]["coming_soon"].GetBool());
				}
				else { game_coming_soon.append(); }

				if (app.HasMember("release_date") && app["release_date"].HasMember("date") && !app["release_date"]["date"].IsNull()) {

					std::string res = reformatDate(app["release_date"]["date"].GetString());
					if (!res.empty()) {
						
						game_release_date.append(res);
					}
					else {
						game_release_date.append();
					}
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

					error = e.what();
					logmsg = "INSERT error: " + error;
					log.post("UpdateGameInfo", appid, logmsg, "error");
					log.status("UpdateGameInfo", 0);
				}



				// Publishers
				if (app.HasMember("publishers")) {
					rapidjson::Value& pubArr = app["publishers"];
					for (rapidjson::SizeType i = 0; i < pubArr.Size(); ++i) {
						pqxx::params publisher_name;
						std::string strsize = pubArr[i].GetString();

						if(strsize.size() > 0) {
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

								error = e.what();
								logmsg = "INSERT error: " + error;
								log.post("UpdateGameInfo", appid, logmsg, "warning");
								log.status("UpdateGameInfo", 0);

							}
						}
						else {
							continue;
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

							error = e.what();
							logmsg = "INSERT error: " + error;
							log.post("UpdateGameInfo", appid, logmsg, "warning");
							log.status("UpdateGameInfo", 0);
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

							error = e.what();
							logmsg = "INSERT error: " + error;
							log.post("UpdateGameInfo", appid, logmsg, "warning");
							log.status("UpdateGameInfo", 0);

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

							error = e.what();
							logmsg = "INSERT error: " + error;
							log.post("UpdateGameInfo", appid, logmsg, "warning");
							log.status("UpdateGameInfo", 0);
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

							error = e.what();
							logmsg = "INSERT error: " + error;
							log.post("UpdateGameInfo", appid, logmsg, "warning");
							log.status("UpdateGameInfo", 0);
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

							error = e.what();
							logmsg = "INSERT error: " + error;
							log.post("UpdateGameInfo", appid, logmsg, "warning");
							log.status("UpdateGameInfo", 0);
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

							error = e.what();
							logmsg = "INSERT error: " + error;
							log.post("UpdateGameInfo", appid, logmsg, "warning");
							log.status("UpdateGameInfo", 0);
						}

						++it;
					}
				}

				txn.commit();

			}

			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);


		}
		catch (const std::exception& e) {
			
			error = e.what();
			logmsg = "INSERT error: " + error;
			log.post("UpdateGameInfo_mainLoop", appid, logmsg, "error");
			log.status("UpdateGameInfo", 0);
		}


	}
	
}

UpdateGameInfo::~UpdateGameInfo()
{
}


std::string UpdateGameInfo::reformatDate(const std::string& dateStr) {
	std::regex monthDayYearRegex(R"((\w{3} \d{1,2}, \d{4}))");
	std::regex monthYearRegex(R"((\w{3} \d{4}))");

	std::smatch match;

	if (std::regex_match(dateStr, match, monthDayYearRegex)) {
		return match[0];
	}

	if (std::regex_match(dateStr, match, monthYearRegex)) {
		std::string matchedMonth = match[1];
		std::string matchedYear = match[2];

		return "01 " + matchedMonth + matchedYear;
	}

	return "";
}