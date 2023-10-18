add_library(func func.cpp)
target_include_directories(func PUBLIC ${CMAKE_SOURCE_DIR}/Games/UpdateGameInfo)

include_directories(${CMAKE_SOURCE_DIR}/Games/UpdateGameInfo)

target_link_libraries(func PRIVATE cURLing CURL::libcurl)
target_link_libraries(func PRIVATE cURLing libpqxx::pqxx)
target_link_libraries(func PRIVATE postgre libpqxx::pqxx)
target_link_libraries(func PRIVATE logger libpqxx::pqxx)


add_executable(UpdateGameInfo UpdateGameInfo.cpp)
target_link_libraries(UpdateGameInfo PRIVATE func libpqxx::pqxx CURL::libcurl)












