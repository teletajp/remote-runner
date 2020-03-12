#pragma once
#include "logger/ilogger.h"
#include "thread/runnable.h"
class MTLogger : public logger::ILogger, public running::IRunnable
{
public:
    MTLogger(const std::string &file_name_base, const std::string & log_dir, size_t max_file_size);
    ~MTLogger();
    void printLog(const std::string &prefix, PL level, const std::string &message) override final;
    void flushLog() override final;
    void updateParams(const std::string &file_name_base, const std::string & log_dir, size_t max_file_size);
    virtual void run(bool &terminated) override final;
private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
