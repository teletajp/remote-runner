#pragma once
#include <memory>
namespace running
{

class IRunnable
{
public:
    virtual ~IRunnable() {};
    virtual void run(bool &terminated) = 0;
};

using RunnablePtr = std::unique_ptr<IRunnable>;

class AutoThread
{
public:
    AutoThread(RunnablePtr&& runnable_object);
    AutoThread(IRunnable* runnable_object);
    ~AutoThread();
    bool terminated() const;
    void terminate();

private:
    AutoThread(const AutoThread&)            = delete;
    AutoThread(AutoThread&&)                 = delete;
    AutoThread& operator=(const AutoThread&) = delete;
    AutoThread& operator=(AutoThread&&)      = delete;
public:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};
}