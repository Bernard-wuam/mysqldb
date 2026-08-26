add_library(MysqlDB INTERFACE)
add_library(MysqlDB::MysqlDB ALIAS MysqlDB)

target_sources(
    MysqlDB
    INTERFACE
    FILE_SET HEADERS TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/mysqldb/mysqldb.h
)

find_package(boost_mysql CONFIG REQUIRED)
find_package(boost_charconv CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(MysqlDB INTERFACE Boost::mysql OpenSSL::SSL
    OpenSSL::Crypto Boost::charconv)
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
