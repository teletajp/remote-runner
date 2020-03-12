#pragma once
#include "select_base_server_engine.h"
#include <thread/runnable.h>
#include <memory>
namespace logger { class ILogger; }
namespace rrd
{
class TcpServer: public running::IRunnable
{
    SelectBaseServerEngine<TcpServer> engine;
public:
    TcpServer(uint16_t port, logger::ILogger * const logger);
    ~TcpServer();

    int onRead(const int fd);
    int onWrite(const int fd);
    int onError(const int fd, const std::string &error_message);
    void run(bool &terminated) override;
private:
    logger::ILogger * const logger_;
    TcpServer(const TcpServer&) = delete;
    TcpServer(TcpServer&&) = delete;
    TcpServer& operator= (const TcpServer&) = delete;
    TcpServer& operator= (TcpServer&&) = delete;
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};
}