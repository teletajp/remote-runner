#pragma once
#include <set>
#include <string>
namespace logger { class ILogger; }
namespace rrd
{
class RemoteRunner
{
public:
    struct Settings
    {
        std::set<std::string> allow_cmds;
        time_t time_out = 0;
        bool is_log_client = false;
        uint16_t port = 12345;
    };

    RemoteRunner(const Settings& setting, logger::ILogger * const logger);
    ~RemoteRunner();
    void run(bool &is_stop);
private:
    Settings setting_;
    logger::ILogger * const logger_;
    RemoteRunner(const RemoteRunner&) = delete;
    RemoteRunner(RemoteRunner&&) = delete;
    RemoteRunner& operator= (const RemoteRunner&) = delete;
    RemoteRunner& operator= (RemoteRunner&&) = delete;
};
}