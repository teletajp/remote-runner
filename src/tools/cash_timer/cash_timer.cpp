#include "cash_timer.h"
#include <thread>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#define pause(millsec) Sleep(millsec);
#endif
#ifdef __linux__
#define pause(millsec) sleep(millsec / 1000);
#endif
cash_timer g_cash_timer;

class cash_timer::impl_t
{
public:
    impl_t():cur_time_(time(nullptr)), mics_(0), cur_time_in_mils_(0), is_run_(false) {};
    ~impl_t() {};
    time_t cur_time_;
    uint32_t mics_;
    time_t cur_time_in_mils_;
    tm local_time_;
    std::thread timer_thread_;
    bool is_run_;
    void run();
};

void cash_timer::impl_t::run()
{
    is_run_ = true;
    decltype(std::chrono::system_clock::now().time_since_epoch()) duration;
    std::chrono::seconds secs;
    std::chrono::milliseconds mils;
    std::chrono::microseconds mics;
    while (is_run_)
    {
        duration = std::chrono::system_clock::now().time_since_epoch();
        secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
        mics = std::chrono::duration_cast<std::chrono::microseconds>(duration) - std::chrono::duration_cast<std::chrono::microseconds>(secs);
        mils = std::chrono::duration_cast<std::chrono::milliseconds>(duration) - std::chrono::duration_cast<std::chrono::milliseconds>(secs);
        mics_ = (uint32_t)mics.count();
        cur_time_ = secs.count();
        cur_time_in_mils_ = cur_time_ *1000 + (uint32_t)mils.count();
        local_time_ = *(localtime(&cur_time_));
        pause(10);
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

cash_timer::cash_timer():
p_impl(new impl_t)
{

}

cash_timer::~cash_timer()
{
    stop();
    delete p_impl;
}

void cash_timer::start()
{
    if (p_impl->timer_thread_.get_id() == std::thread().get_id())
        p_impl->timer_thread_ = std::thread(&impl_t::run, &(*p_impl));
}

void cash_timer::stop()
{
    p_impl->is_run_ = false;
    if(p_impl->timer_thread_.joinable())
        p_impl->timer_thread_.join();
}

time_t cash_timer::get_time()
{
    return p_impl->cur_time_;
}

tm cash_timer::get_local_time()
{
    return p_impl->local_time_;
}

uint32_t cash_timer::get_mics()
{
    return p_impl->mics_;
}

time_t cash_timer::get_time_in_mils()
{
    return p_impl->cur_time_in_mils_;
}

void cash_timer::get_time(uint32_t & sec, uint32_t & usec)
{
    sec = (uint32_t)p_impl->cur_time_;
    usec = p_impl->mics_;
}

