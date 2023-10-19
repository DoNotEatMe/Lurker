add_library(cURLing "cURLing.cpp" )
target_include_directories(cURLing PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/cURLing)

target_link_libraries(cURLing PRIVATE CURL::libcurl logger libpqxx::pqxx)





