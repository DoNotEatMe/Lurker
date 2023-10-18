cmake_minimum_required (VERSION 3.26)

project (Lurker)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(VCPKG_ROOT "D:/vcpkg/")
    #set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
else()

    set(VCPKG_ROOT "/root/vcpkg")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
    #set(CMAKE_TOOLCHAIN_FILE "/root/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out/install")

find_package(CURL CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

add_subdirectory(Games)
add_subdirectory(Credentials)
add_subdirectory(Reccomendations)
