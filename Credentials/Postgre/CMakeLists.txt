add_library(postgre Postgre.cpp )
	find_package(libpqxx CONFIG REQUIRED)
	target_link_libraries(postgre PRIVATE libpqxx::pqxx)

target_include_directories(postgre PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/Postrgre ${CMAKE_SOURCE_DIR}/Credentials/)