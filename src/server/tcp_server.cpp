#include <iostream>
#include <map>
#include <fmt/format.h>
#include <logger/ilogger.h>
#include "tcp_server.h"
#include "connection.h"
#include "listener.h"

#define printLog(level, message) if (logger_) {logger_->printLog("TcpServer", level, message);}

namespace rrd
{
class TcpServer::Impl
{
public:
    Impl(uint16_t port);
    ~Impl();

    uint16_t port_;
    Listener listener_;
    std::map<int, std::unique_ptr<Connection>> all_connections_;
};

TcpServer::Impl::Impl(uint16_t port):
port_(port),
listener_(port, true)
{
}

TcpServer::Impl::~Impl()
{
}


TcpServer::TcpServer(uint16_t port, logger::ILogger * const logger):
logger_(logger),
pimpl_(std::make_unique<Impl>(port))
{
    engine.setReadCb(RCallBack<TcpServer>(this, &TcpServer::onRead));
    engine.setWriteCb(WCallBack<TcpServer>(this, &TcpServer::onWrite));
    engine.setErrorCb(ECallBack<TcpServer>(this, &TcpServer::onError));
}
TcpServer::~TcpServer()
{
}
int TcpServer::onRead(const int fd)
{
    if (fd == pimpl_->listener_.getSocket())
    {
        std::string info;
        const int new_fd = pimpl_->listener_.accept(info);
        if (new_fd == INVALID_SOCKET)
        {
            printLog(PL::ERRO, info);
        }
        else
        {
            printLog(PL::NOTICE, "New connection " + info);
            if (!pimpl_->all_connections_.emplace(new_fd, std::make_unique<Connection>(new_fd, info)).second)
            {
                printLog(PL::CRIT, fmt::format("Connection {} always exist. Logic error.", info));
            }
        }
    }
    else
    {
        auto find_it = pimpl_->all_connections_.find(fd);
        if (find_it == pimpl_->all_connections_.end())
        {
            printLog(PL::CRIT, "Connection not exist. Logic error.");
        }
        else
        {
            auto &connection = find_it->second;
            //connection->receive();
        }
    }
    return 0;
}
int TcpServer::onWrite(const int fd)
{
    //std::cout << fmt::format("{}", fd) << std::endl;
    return 0;
}
int TcpServer::onError(const int fd, const std::string &error_message)
{
    if (fd == pimpl_->listener_.getSocket())
    {
        printLog(PL::ERRO, "Error on listen socket " + error_message);
    }
    return 0;
}
void TcpServer::run(bool & terminate)
{
    if (!pimpl_->listener_.isValid())
    {
        printLog(PL::FATAL, "Listener not valid. Check port setting.");
    }
    else
    {
        printLog(PL::NOTICE, "Server started");
        if (!engine.addSocket(pimpl_->listener_.getSocket()))
        {
            printLog(PL::FATAL, "Server engine can't add listen socket.");
        }
        else
        {
            while (!terminate)
            {
                engine.nextLoop();
            }
        }
    }
    printLog(PL::NOTICE, "Server stoped");
}
}