#include <iostream>
#include <fstream>
#include <fmt/format.h>
#include <logger/mt_logger.h>
#include <cash_timer/cash_timer.h>
#include "remote_runner/remote_runner.h"

bool g_stop_program = false;

#ifdef _WIN32
#include <Windows.h>
BOOL __stdcall CloseHandlerFn(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
        return TRUE;
    case CTRL_BREAK_EVENT:
        return TRUE;
    case CTRL_CLOSE_EVENT:
        g_stop_program = true;
        ExitThread(0);
        //return FALSE;
        //case CTRL_SHUTDOWN_EVENT:
        //	Sleep(5000);
        break;
    default: break;
    };
    return FALSE;
}
#endif

static bool init_startup_params(int ac, char** av, rrd::RemoteRunner::Settings &rr_setting)
{
    const int mandatory_param_need = 1;
    const int max_command_len = 128;
    std::string param;
    int mandatory_param_count = 0;
    
    for (int i = 1; i < ac; ++i)
    {
        param = av[i];
        if (param.find("--allow_cmds_file=") != std::string::npos)
        {
            std::string file_name = param.substr(18);
            if (file_name.empty()) return false;

            std::ifstream file_stream(file_name);
            if (!file_stream.is_open()) return false;

            for (std::string cmd; std::getline(file_stream, cmd);)
            {
                if (cmd.length() <= max_command_len && cmd.find_first_of(" \t") == std::string::npos)
                    rr_setting.allow_cmds.emplace(cmd);
            }
            file_stream.close();
            mandatory_param_count++;
        }
        else  if (param.find("--time_out=") != std::string::npos)
        {
            try
            {
                rr_setting.time_out = static_cast<time_t>(std::stoi(param.substr(11), 0, 10));
            }
            catch (...) { return false; };
        }
        else  if (param.find("--log") != std::string::npos)
        {
            rr_setting.is_log_client = true;
        }
    }

    if (mandatory_param_count != mandatory_param_need) return false;
    return true;
}

int main(int argc, char** argv)
{
#ifdef WIN32
    SystemParametersInfoA(SPI_GETHUNGAPPTIMEOUT, 40000, NULL, 0);
    SetConsoleCtrlHandler(CloseHandlerFn, TRUE);
#endif
    rrd::RemoteRunner::Settings rr_setting;
    std::string full_program_name(argv[0]);
    std::string program_name = full_program_name.substr(full_program_name.rfind('\\') + 1);
    std::string program_path = full_program_name.substr(0, full_program_name.rfind('\\'));
    std::string base = program_name + ".log";
    g_cash_timer.start();
    {
        logget_ptr_t logger = std::make_shared<MTLogger>(base, program_path, size_t(1024UL * 1024UL * 2048UL));
        std::unique_ptr<running::AutoThread> logger_thread;
        logger_thread = std::make_unique<running::AutoThread>(dynamic_cast<running::IRunnable*>((MTLogger*)&(*logger)));
        if (!init_startup_params(argc, argv, rr_setting))
        {
            logger->printLog(program_name, PL::FATAL, fmt::format("Syntax of command:\n \t{} --allow_cmds_file=\"cmd_file_name\" [--time_out=15] [--log]", program_name));
        }
        else
        {
            rrd::RemoteRunner rr(rr_setting, logger.get());
            rr.run(g_stop_program);
        }
    }
    g_cash_timer.stop();
	return 0;
}