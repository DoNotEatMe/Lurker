add_library(postgre Postgre.cpp )

target_link_libraries(postgre PRIVATE libpqxx::pqxx)

target_include_directories(postgre PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/Postgre ${CMAKE_SOURCE_DIR}/Credentials/)