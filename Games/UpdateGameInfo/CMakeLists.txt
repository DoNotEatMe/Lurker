find_package(CURL CONFIG REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(RapidJSON CONFIG REQUIRED)
include_directories(${RapidJSON_INCLUDE_DIRS})

find_package(libpqxx CONFIG REQUIRED)

add_library(func func.cpp)
target_include_directories(func PUBLIC ${CMAKE_SOURCE_DIR}/Games/UpdateGameInfo)

include_directories(${CMAKE_SOURCE_DIR}/Games/UpdateGameInfo)

target_link_libraries(func PRIVATE cURLing CURL::libcurl)
target_link_libraries(func PRIVATE postgre libpqxx::pqxx)
target_link_libraries(func PRIVATE logger libpqxx::pqxx)


add_executable(UpdateGameInfo UpdateGameInfo.cpp)
target_link_libraries(UpdateGameInfo PRIVATE func libpqxx::pqxx CURL::libcurl)












