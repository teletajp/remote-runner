#pragma once
#include <ctime>
#include <cstdint>
class cash_timer
{
public:
    cash_timer();
    ~cash_timer();
    void start();
    void stop();
    time_t get_time();
    tm get_local_time();
    uint32_t get_mics();
    time_t get_time_in_mils();
    void get_time(uint32_t &sec, uint32_t &usec);
private:
    class impl_t;
    impl_t *p_impl;
};

extern cash_timer g_cash_timer;