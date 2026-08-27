#pragma once

#include <boost/asio.hpp>
#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/json.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/connect_params.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/diagnostics.hpp>
#include <boost/mysql/error_code.hpp>
#include <boost/mysql/pool_params.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/static_results.hpp>
#include <boost/mysql/with_params.hpp>
#include <memory>
#include <stdexcept>

class MysqlDB {
public:
  MysqlDB(const MysqlDB &others) = delete;
  MysqlDB &operator=(const MysqlDB &others) = delete;
  MysqlDB &operator=(MysqlDB &&others) = delete;
  MysqlDB(MysqlDB &&others) = delete;

private:
  MysqlDB() = default;
  bool isInitialized = false;
  std::unique_ptr<boost::mysql::connection_pool> m_connPool = nullptr;

  static MysqlDB &getInstance() {
    static MysqlDB instance;
    return instance;
  }

public:
  static boost::mysql::connection_pool &getConnectionPool() {
    MysqlDB &instance = MysqlDB::getInstance();
    if (!instance.m_connPool)
      throw(std::logic_error("not initialized"));
    return *instance.m_connPool;
  }

  static void init(boost::asio::thread_pool &pool, const std::string hostname,
                   unsigned short port, const std::string &password,
                   const std::string &username, const std::string &database,
                   bool threadsafe = true, bool multiqueries = true) {
    MysqlDB &instance = getInstance();
    if (MysqlDB::getInstance().isInitialized) {
      throw(std::logic_error("not initialized"));
    }

    boost::mysql::pool_params poolParams;
    poolParams.server_address.emplace_host_and_port(hostname, port);
    poolParams.password = password;
    poolParams.username = username;
    poolParams.database = database;
    poolParams.thread_safe = threadsafe;
    poolParams.multi_queries = multiqueries;
    instance.m_connPool = std::make_unique<boost::mysql::connection_pool>(
        pool, std::move(poolParams));

    instance.isInitialized = true;
  }
};