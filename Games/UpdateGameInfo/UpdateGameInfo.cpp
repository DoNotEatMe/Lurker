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
	Postgre DB;
	//DB.TablesCheck();

	int appid = 594650;
	

	std::string link = "https://store.steampowered.com/api/appdetails?appids=" + std::to_string(appid) + "&&cc=US";

	auto start_time = std::chrono::high_resolution_clock::now();

	cURLing curl;
	curl.getHTML(link.c_str());

	rapidjson::Document document;
	document.Parse(curl.GetBuffer().c_str());

	if ( document[std::to_string(appid).c_str()].IsObject() && document[std::to_string(appid).c_str()]["success"].GetBool() == true ) {

		rapidjson::Value& app = document[std::to_string(appid).c_str()]["data"];

		std::cout << "main game info block" << std::endl << std::endl
			<< "type: " << app["type"].GetString() << std::endl
			<< "name: " << app["name"].GetString() << std::endl
			<< "is_free: " << app["is_free"].GetBool() << std::endl
			<< "short_descripton: " << app["short_description"].GetString() << std::endl
			<< std::endl
			<< "detailed_description: " << app["detailed_description"].GetString() << std::endl
			<< std::endl
			<< "website: " << app["website"].GetString() << std::endl
			<< "price: " << app["price_overview"]["initial"].GetDouble() << std::endl
			<< "windows: " << app["platforms"]["windows"].GetBool() << std::endl
			<< "mac: " << app["platforms"]["mac"].GetBool() << std::endl
			<< "linux: " << app["platforms"]["linux"].GetBool() << std::endl
			<< "metacritic_score: " << app["metacritic"]["score"].GetInt() << std::endl
			<< "total_recommendations: " << app["recommendations"]["total"].GetInt() << std::endl
			<< "coming_soon: " << app["release_date"]["coming_soon"].GetBool() << std::endl
			<< "release_date: " << app["release_date"]["date"].GetString() << std::endl
			<< "support_url: " << app["support_info"]["url"].GetString() << std::endl
			<< "support_email: " << app["support_info"]["email"].GetString() << std::endl;



		//TODO: Categories
		std::cout << std::endl;
		std::cout << "categories: " << std::endl << std::endl;
		rapidjson::Value& catArr = app["categories"];
		for (rapidjson::SizeType i = 0; i < catArr.Size(); ++i) {
			std::cout << "id: " << catArr[i]["id"].GetInt() << " desc: " << catArr[i]["description"].GetString() << std::endl;
		}
		
		

		//TODO: Genres
		std::cout << std::endl;
		std::cout << "genres: " << std::endl << std::endl;
		rapidjson::Value& genArr = app["genres"];
		for (rapidjson::SizeType i = 0; i < genArr.Size(); i++) {
			std::cout << "id: " << genArr[i]["id"].GetString() << " desc: " << genArr[i]["description"].GetString() << std::endl;
		}

		//TODO: Developers
		std::cout << std::endl;
		std::cout << "developers: " << std::endl << std::endl;
		rapidjson::Value& devArr = app["developers"];
		for (rapidjson::SizeType i = 0; i < devArr.Size(); ++i) {
			std::cout << "dev: " << devArr[i].GetString() << std::endl;
		}

		//TODO: Publishers
		std::cout << std::endl;
		std::cout << "publishers: " << std::endl << std::endl;
		rapidjson::Value& pubArr = app["publishers"];
		for (rapidjson::SizeType i = 0; i < pubArr.Size(); ++i) {
			std::cout << "pub: " << pubArr[i].GetString() << std::endl;
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




		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		std::cout << std::endl;
		std::cout << "Query execution time: " << duration.count() << " ms" << std::endl;

	};

}

