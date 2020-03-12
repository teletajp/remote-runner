cmake_minimum_required(VERSION 3.12.0)

#CMAKE_TOOLCHAIN_FILE должен определяться раньше project
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

STRING(REPLACE "\\" "/" VCPKG_ROOTM $ENV{VCPKG_ROOT} )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateTargetTriplet)
if(NOT DEFINED PKG_TARGET_TRIPLET)
	GENERATE_TARGET_TRIPLET()
endif()

project(remote_runnerd VERSION 1.0.0.1 LANGUAGES CXX)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>bin")
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>bin")
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib")
endif()

#U_PROJECT_NAME используется для формирования версии в version.h.in
string(TOUPPER ${PROJECT_NAME} U_PROJECT_NAME)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")

add_subdirectory("src")
#Add test project
find_package(GTest MODULE)
find_package(Threads MODULE)
if(GTest_FOUND AND Threads_FOUND)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Gtest not found! Tests will not build.")
endif()