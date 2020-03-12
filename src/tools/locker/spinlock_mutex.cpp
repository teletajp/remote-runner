#include "spinlock_mutex.h"
#include <atomic>
#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#define pause(millsec)   Sleep(millsec);
#endif
#ifdef __linux__
#define pause(millsec) sleep(millsec / 1000);
#endif
class spinlock_mutex::impl_t
{
public:
    impl_t():flag(){};
    ~impl_t(){};
    std::atomic_flag flag;
};

spinlock_mutex::spinlock_mutex():
p_impl(new impl_t)
{
}

spinlock_mutex::~spinlock_mutex()
{
    delete p_impl;
}

void spinlock_mutex::lock()
{
    while (p_impl->flag.test_and_set(std::memory_order_acquire))
    {
        pause(0);
    }
}

void spinlock_mutex::unlock()
{
    p_impl->flag.clear(std::memory_order_release);
}
