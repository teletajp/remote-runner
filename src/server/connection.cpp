#include "connection.h"
#include "newtork_initializer.h"
namespace rrd
{
class Connection::Impl
{
public:
    Impl(int fd, const std::string &info);
    ~Impl();
    bool isInit() { return fd_ != INVALID_SOCKET; }
    int fd_;
    std::string info_;

};

Connection::Impl::Impl(int fd, const std::string &info):
fd_(fd),
info_(info)
{
}
Connection::Impl::~Impl()
{
    if (fd_ != INVALID_SOCKET)
    {
        shutdown(fd_, SD_BOTH);
        closesocket(fd_);
    }
}

Connection::Connection(int fd, const std::string &info):
pimpl_(std::make_unique<Impl>(fd, info))//Д.б. последним
{
}

Connection::~Connection()
{
}

Connection::Connection(Connection && con)
{
    pimpl_.swap(con.pimpl_);
}

int Connection::receive()
{
    if (!pimpl_->isInit()) return 0;
}
const std::string & rrd::Connection::getInfo() const
{
    return pimpl_->info_;
}
}