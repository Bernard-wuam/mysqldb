cmake_minimum_required(VERSION 3.10.0)
project(mysqldb VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(MysqlDB INTERFACE)
add_library(MysqlDB::MysqlDB ALIAS MysqlDB)

target_sources(
    MysqlDB
    INTERFACE
    FILE_SET HEADERS TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/mysqldb/mysqldb.h
)

install(
    TARGETS MysqlDB
    EXPORT MysqlDBTargets
    FILE_SET HEADERS
)

install(
    EXPORT MysqlDBTargets
    DESTINATION share/cmake/mysqldb
    NAMESPACE MysqlDB::
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/mysqldbConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MysqlDBConfig.cmake
    INSTALL_DESTINATION share/cmake/mysqldb)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/MysqlDBConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MysqlDBConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MysqlDBConfigVersion.cmake
    DESTINATION share/cmake/mysqldb
)