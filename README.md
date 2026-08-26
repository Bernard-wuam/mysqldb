# mysqldb
#include "mysqldb/mysqldb.h"
#include <iostream>

boost::asio::awaitable<void> run() {
  std::cout << "started coroutine" << std::endl;

  // auto conn = co_await connPool.async_get_connection();
  auto conn = co_await MysqlDB::getConnectionPool()->async_get_connection();
  boost::mysql::diagnostics dail;
  boost::mysql::error_code ec;

  boost::mysql::results result;
  co_await conn->async_execute(
      boost::mysql::with_params(
          R"(SELECT userName FROM users WHERE users.publicUserId = {};)",
          "16e7d100-9e62-11f1-9bd0-00155d51cbc3"),
      result, dail, boost::asio::redirect_error(ec));
  if (ec) {
    std::cerr << ec.what() << "\n";
    co_return;
  }
  std::cout << "no error" << std::endl;
  if (result.rows().empty()) {
    std::cerr << " record not found \n";
    co_return;
  }
  auto val = result.rows().at(0).at(0).as_string();
  std::cout << val << "\n";

  co_return;
}

int main(int, char **) {

 boost::asio::io_context ioc;
  MysqlDB::init(ioc, "localhost", 33061, "1914", "root", "blogdev");
  boost::mysql::pool_params poolParams;


  boost::mysql::connection_pool connPool(ioc, std::move(poolParams));

  boost::asio::co_spawn(ioc, run(), [](std::exception_ptr e) {
    if (e) {
      try {
        std::rethrow_exception(e);
      } catch (const std::exception &ec) {
        std::cerr << ec.what() << "\n";
        return;
      }
    }
  });
  MysqlDB::getConnectionPool()->async_run(boost::asio::detached);
  ioc.run();


  std::cout << "Hello, from kt!\n";
  return 0;
}