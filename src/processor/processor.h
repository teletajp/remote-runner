#pragma once
#include <set>
#include <thread/runnable.h>
namespace rrd
{
class Processor : public running::IRunnable
{
    enum class TaskStatus
    {
        valid,
        invalid,
    };
    struct Settings
    {
        std::set<std::string> allow_cmds;
        time_t time_out = 0;
        bool is_log_client = false;
        Settings(const std::set<std::string> &allow_cmds, time_t time_out, bool is_log_client) :allow_cmds(allow_cmds), time_out(time_out), is_log_client(is_log_client) {}
    };
public:

    Processor(const std::set<std::string> &allow_cmds, time_t time_out, bool is_log_client);
    void run(bool &terminated) override;
    Processor::TaskStatus addTask(int fd, std::string cmd, time_t timeout);
private:
    Settings settings_;
    Processor(const Processor&) = delete;
    Processor(Processor&&) = delete;
    Processor& operator= (const Processor&) = delete;
    Processor& operator= (Processor&&) = delete;
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
}