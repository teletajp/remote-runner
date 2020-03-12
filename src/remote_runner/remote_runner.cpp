#include "remote_runner.h"
#include "processor/processor.h"
#include "server/tcp_server.h"
#include <cash_timer/cash_timer.h>
#include <logger/ilogger.h>

#define printLog(level, message) if (logger_) {logger_->printLog("RemoteRunner", level, message);}
namespace rrd
{
RemoteRunner::RemoteRunner(const Settings & setting, logger::ILogger * const logger):
setting_(setting_),
logger_(logger)
{
}

RemoteRunner::~RemoteRunner()
{
}

void RemoteRunner::run(bool &is_stop)
{
    TcpServer server(12345, logger_);
    printLog(PL::NOTICE, "RemoteRunner start");
    server.run(is_stop);
    printLog(PL::NOTICE, "RemoteRunner stop");
}
}