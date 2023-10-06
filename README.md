# LurkerNew
WIP
Server app for parsing steam data from steam api (currently only steam api) and storing in postgres

Current stage:
cURLing - class represents connections with http.
Postgres - DB connection holder with DB struct creation if needed.
Logger - simple logger stored info in DB.
InitAllGames - grab banch of appid and game titles and store in db. Around 170k rows. Could be not full ion case of steam api features. Purposed to fill initial game list every N time.
UpdateGameInfo - grab every game via steam api, struct info and place in DB tables. 

vcpkg dependencies:
cURL
pqxx
rapidjson

TODO:
*looping for game info and applist update. need some research to manage perfomance. In case of ~0.6s on game push for 170k+ games.
*learn about SteamKit and try to reproduce it with c++
*comment grabber
*games online/subscribers && any other dynamic data that could be usefull in graphs
*increment html parsing library to grab additional data from stem http
