add_library(cURLing "cURLing.cpp" )
target_include_directories(cURLing PUBLIC ${CMAKE_SOURCE_DIR}/Credentials/cURLing)


#include_directories(${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)

find_package(CURL CONFIG REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})





