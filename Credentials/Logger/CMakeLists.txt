add_library(logger Logger.cpp )
target_include_directories(logger PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/Logger ${CMAKE_SOURCE_DIR}/Credentials/ ${CMAKE_SOURCE_DIR}/Credentials/Postgre)

target_link_libraries(logger PRIVATE libpqxx::pqxx)