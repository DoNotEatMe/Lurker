add_executable(InitAllGames InitAllGames.cpp)

target_link_libraries(InitAllGames PRIVATE cURLing CURL::libcurl)
target_link_libraries(InitAllGames PRIVATE postgre libpqxx::pqxx)

install(TARGETS InitAllGames DESTINATION /root/LurkerExec/InitAllGames/)