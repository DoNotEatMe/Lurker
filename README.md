# About

**Lurker** planned as complex server app to parse steam data and store into DB inspired by SteamDB and SteamSpy.
Project was created in educational purposes. 
Lurker is pretty shallow now, but I have vision to work on.


## Current stage
* Common libs
	* [cURLing](/Credentials/cURLing/cURLing.cpp) - cURL returning buffer with page data
	* [Logger](/Credentials/Logger/Logger.cpp) - simple class to store logs to DB
	* [Postgre](/Credentials/Postgre/Postgre.cpp) - DB connection holder (/Credentials/credentials.cpp reqiured)
* Common apps
	* [InitAllGames](/Games/InitAllGames/InitAllGames.cpp) - Get gamelist feom steam web api and insert appid and game title to DB
	* [UpdateGameInfo](/Games/UpdateGameInfo/UpdateGameInfo.cpp) - Get list of games in DB (after InitAllGames) and push to DB info from steam web api



### Dependencies
`vcpkg install curl`

`vcpkg install libpqxx`

`vcpkg install rapidjson`

## Working on:

* After initial UpdateGameInfo complete - make upper-analytics for efficiency decision
* Learn [SteamKit](https://github.com/SteamRE/SteamKit) and try to realise usefull methods in Lurker
* Pricing for each region
* Tags for games
* Live Online
* Comments for games

### Longterms

* Somithing like SteamKit listner to Steam changes. Need to research steam callbacs and 
* Web interface for "Steam Pulse" (upper level analytics of platform e.g. games/year, games/languages e.t.c)
* Steamworks csv analytics (Steam traffic, Pricing, wishlists/cohort)
 

#### Known issues
* Language incorrect regex, so bad DB results.
* too many 429. Proxy holder needed or SteamKitResearch.
