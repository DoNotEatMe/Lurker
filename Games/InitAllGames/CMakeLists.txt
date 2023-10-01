add_executable(InitAllGames InitAllGames.cpp)

find_package(CURL CONFIG REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(RapidJSON CONFIG REQUIRED)
include_directories(${RapidJSON_INCLUDE_DIRS})

find_package(libpqxx CONFIG REQUIRED)

target_link_libraries(InitAllGames PRIVATE cURLing CURL::libcurl)
target_link_libraries(InitAllGames PRIVATE postgre libpqxx::pqxx)