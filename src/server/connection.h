#pragma once
#include <string>
namespace rrd
{
class Connection
{
public:
    Connection(int fd, const std::string &info);
    ~Connection();
    Connection(Connection&& con);
    int receive();
    const std::string &getInfo() const;
private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

}