add_executable(UpdateGameInfo UpdateGameInfo.cpp)

add_library(func func.cpp )
target_include_directories(func PUBLIC ${CMAKE_SOURCE_DIR}/games/updategameinfo)

find_package(CURL CONFIG REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(RapidJSON CONFIG REQUIRED)
include_directories(${RapidJSON_INCLUDE_DIRS})

find_package(libpqxx CONFIG REQUIRED)

target_link_libraries(func PRIVATE cURLing CURL::libcurl)
target_link_libraries(func PRIVATE postgre libpqxx::pqxx)
target_link_libraries(func PRIVATE logger libpqxx::pqxx)