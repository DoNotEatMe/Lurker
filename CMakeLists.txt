cmake_minimum_required (VERSION 3.26)

project (Lurker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(Games)
add_subdirectory(Credentials)
add_subdirectory(Reccomendations)

message("Found PostgreSQL: ${libpqxx_VERSION}")
message("Current libpqxx_INCLUDE_DIRS File: ${libpqxx_INCLUDE_DIRS}")
message("Current CMAKE_SOURCE_DIR File:${CMAKE_SOURCE_DIR}")


