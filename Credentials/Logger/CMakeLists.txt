add_library(logger logger.cpp )
target_include_directories(logger PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/Logger ${CMAKE_SOURCE_DIR}/Credentials/)

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(logger PRIVATE libpqxx::pqxx)