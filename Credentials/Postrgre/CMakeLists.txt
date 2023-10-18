add_library(postgre Postgre.cpp )
target_include_directories(postgre PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/Postrgre ${CMAKE_SOURCE_DIR}/Credentials/)

target_link_libraries(postgre PRIVATE libpqxx::pqxx)

