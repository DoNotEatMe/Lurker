cmake_minimum_required (VERSION 3.28.0)

project (Lurker)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(VCPKG_ROOT "D:/vcpkg/")
    set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
else()
    set (CURL_DIR "/usr/local/share/vcpkg/installed/x64-linux/share/curl/")
    set (RapidJSON_DIR "/usr/local/share/vcpkg/installed/x64-linux/share/rapidjson/")
    set (libpqxx_DIR "/usr/local/share/vcpkg/installed/x64-linux/share/libpqxx/")
    set (PostgreSQL_INCLUDE_DIR "/usr/local/share/vcpkg/installed/x64-linux/share/postgresql/")
endif()


find_package(CURL CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

add_subdirectory(Games)
add_subdirectory(Credentials)
add_subdirectory(Reccomendations)
