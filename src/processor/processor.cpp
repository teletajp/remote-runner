#include "processor.h"
#include <locker/spinlock_mutex.h>
#include <atomic>
#include <list>
#include <mutex>
#ifdef _WIN32
#include <windows.h>
#define pause(millsec)   Sleep(millsec);
#endif
#ifdef __linux__
#define pause(millsec) sleep(millsec / 1000);
#endif
namespace rrd
{

struct Task
{
    int fd;
    std::string command;
    time_t timeout;
    Task(int fd, std::string &&command, time_t timeout) :fd(fd), command(command), timeout(timeout) {};
};

class Processor::Pimpl
{
public:
    Pimpl() {};
    ~Pimpl() {};
    spinlock_mutex spin_lock_;
    std::atomic_bool has_new_commands_;
    std::list<Task> temp_tasks_;
    std::list<Task> processing_tasks_;
    void loadNewTasks();
    void processTasks();
};

void Processor::Pimpl::loadNewTasks()
{
    if (has_new_commands_.load())
    {
        std::lock_guard<spinlock_mutex> lock(spin_lock_);
        processing_tasks_.splice(processing_tasks_.end(), temp_tasks_);
        has_new_commands_.store(false);
        bool execCommand(const std::string& cmd);
    }
}
void Processor::Pimpl::processTasks()
{
    auto end = processing_tasks_.end();
    for (auto it = processing_tasks_.begin(); it != end;)
    {
        execCommand(it->command);
    }
}
bool execCommand(const std::string& cmd)
{
#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL,   // No module name (use command line)
             const_cast<LPSTR>(cmd.c_str()),        // Command line
             NULL,           // Process handle not inheritable
             NULL,           // Thread handle not inheritable
             FALSE,          // Set handle inheritance to FALSE
             0,              // No creation flags
             NULL,           // Use parent's environment block
             NULL,           // Use parent's starting directory 
             &si,            // Pointer to STARTUPINFO structure
             &pi)           // Pointer to PROCESS_INFORMATION structure
        ) return false;
    // Wait until child process exits.
    //WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#endif // _WIN32
#ifdef __linux__
    ????
    pid_t pid = fork();
    if (pid == -1) return false;
    if (pid == 0)//child
    {
        auto pos_cmd_end = cmd.find_first_of(" \t");
        auto pos_arg_beg = cmd.find_first_not_of(" \t", pos_cmd_end);
        auto command = cmd.substr(0, pos_cmd_end);
        std::string args;
        if (pos_arg_beg != std::string::npos)
        {
            args = cmd.substr(pos_arg_beg);
        }
        execl(command.c_str(), command.c_str(), args.c_str());
        pid = getpid();
        kill(pid, )
    }
#endif
    return true;
}
Processor::Processor(const std::set<std::string> &allow_cmds, time_t time_out, bool is_log_client):
settings_(allow_cmds, time_out, is_log_client),
pimpl_(std::make_unique<Pimpl>())
{
}

void Processor::run(bool & terminate)
{
    while (!terminate)
    {
        pause(1000);
        pimpl_->loadNewTasks();
        pimpl_->processTasks();
        if (!pimpl_->processing_tasks_.empty())
        {
            
        }
    }
}
Processor::TaskStatus Processor::addTask(int fd, std::string cmd, time_t timeout)
{
    std::lock_guard<spinlock_mutex> lock(pimpl_->spin_lock_);
    if (cmd.empty()) return Processor::TaskStatus::invalid;

    if( settings_.allow_cmds.find(cmd.substr(0 , cmd.find_first_of(" \t"))) == settings_.allow_cmds.end())
        return Processor::TaskStatus::invalid;
    
    pimpl_->temp_tasks_.emplace_back(fd, std::move(cmd), timeout);
    pimpl_->has_new_commands_.store(true);
    return Processor::TaskStatus::valid;
}
}