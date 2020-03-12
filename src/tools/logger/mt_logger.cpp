#include <fmt/time.h>
#include <fstream>
#include <list>
#include <mutex>
//#include <Windows.h>
#include "locker/spinlock_mutex.h"
#include "cash_timer/cash_timer.h"
#include "mt_logger.h"
class MTLogger::Pimpl
{
public:
    Pimpl(const std::string &file_name_base, const std::string & log_dir, size_t max_file_size);
    ~Pimpl();
    struct PrintLogData
    {
        std::string prefix; PL level; std::string message;
        PrintLogData() :level(PL::INFO){}
    };
    PrintLogData last_printed_data_;
    uint64_t repite_counter_;
    time_t rept_time_;

    spinlock_mutex logger_mut_;
    size_t tmp_log_size_;

    std::list<std::string> tmp_log_;
    std::list<std::string> work_log_;
    std::ofstream file_;
    int file_day_;
    size_t file_size_;
    std::string file_name_base_;
    std::string file_ext_;
    std::string log_dir_;
    size_t max_file_size_;
    uint8_t file_indx_;
    bool writeToFile(std::string log_str);
};

MTLogger::Pimpl::Pimpl(const std::string &file_name_base, const std::string & log_dir, size_t max_file_size):
last_printed_data_(),
repite_counter_(0),
rept_time_(0),
tmp_log_size_(0),
file_day_(0),
file_size_(0),
file_name_base_(file_name_base),
file_ext_("log"),
log_dir_(log_dir),
max_file_size_(max_file_size),
file_indx_(0)
{
    if (log_dir_.empty())
        log_dir_ = ".\\";
    else if (log_dir_.back() != '\\')
        log_dir_.append("\\");

    auto dot_pos = file_name_base.rfind(".");
    if (dot_pos != std::string::npos)
    {
        file_ext_ = file_name_base_.substr(dot_pos+1);
        if (file_ext_.length() > 3)
            file_ext_ = "log";
        else
            file_name_base_ = file_name_base_.substr(0, dot_pos);
    }
}

MTLogger::Pimpl::~Pimpl()
{
    if (file_.is_open())
    {
        file_.flush();
        file_.close();
    }
}

bool MTLogger::Pimpl::writeToFile(std::string log_str)
{
    tm cur_tm = g_cash_timer.get_local_time();
    if (cur_tm.tm_mday != file_day_)
    {
        file_indx_ = 0;
        file_.flush();
        file_.close();
    }
    if (!file_.is_open())
    {
        std::string file_name = fmt::format("{}{}_{:04d}{:02d}{:02d}_{:02d}{:02d}{:02d}.{}", log_dir_, file_name_base_, cur_tm.tm_year - 100 + 2000, cur_tm.tm_mon + 1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec, file_ext_);
        file_.open(file_name, std::ios_base::app);
        file_day_ = cur_tm.tm_mday;
        file_size_ = file_.tellp();
    }

    if (file_.is_open())
    {
        file_ << log_str << "\n";
        file_size_ = file_.tellp();
        if (file_size_ >= max_file_size_)
        {
            file_.flush();
            file_.close();
        }
        return true;
    }
    return false;
}

MTLogger::MTLogger(const std::string &file_name_base, const std::string & log_dir, size_t max_file_size):
pimpl_(std::make_unique<Pimpl>(file_name_base, log_dir, max_file_size))
{

}

MTLogger::~MTLogger()
{
}

void MTLogger::printLog(const std::string &prefix, PL level, const std::string &message)
{
    time_t cur_time = g_cash_timer.get_time();
    uint32_t mils = g_cash_timer.get_mics() / 1000;
    std::lock_guard <spinlock_mutex> lock_(pimpl_->logger_mut_);
    try
    {
        if (pimpl_->last_printed_data_.prefix == prefix &&
            pimpl_->last_printed_data_.level == level &&
            pimpl_->last_printed_data_.message == message)
        {
            if (pimpl_->repite_counter_ == 0) pimpl_->rept_time_ = cur_time;
            ++pimpl_->repite_counter_;
            double delta = difftime(cur_time, pimpl_->rept_time_);
            if (delta >= 10)
            {
                std::string print_msg(fmt::format("[{0:%d.%m.%y %H:%M:%S}:{1:<03d}] {2} <{3}> {4} (REPT:{5})", *std::localtime(&cur_time), mils,
                                                  PRIOR2STR[(uint8_t)level], prefix, message, pimpl_->repite_counter_));

                pimpl_->tmp_log_.emplace_back(std::move(print_msg));
                pimpl_->tmp_log_size_++;
                pimpl_->repite_counter_ = 0;
            }
            else if (delta <= 0)
                pimpl_->rept_time_ = cur_time;
        }
        else
        {
            if (pimpl_->repite_counter_)
            {
                std::string print_msg(fmt::format("[{0:%d.%m.%y %H:%M:%S}:{1:<03d}] {2} <{3}> {4} (REPT:{5})", *std::localtime(&cur_time), mils,
                                        PRIOR2STR[(uint8_t)pimpl_->last_printed_data_.level], pimpl_->last_printed_data_.prefix,
                                        pimpl_->last_printed_data_.message, pimpl_->repite_counter_));

                pimpl_->tmp_log_.emplace_back(std::move(print_msg));
                pimpl_->tmp_log_size_++;
                pimpl_->repite_counter_ = 0;
            }
            std::string print_msg(fmt::format("[{0:%d.%m.%y %H:%M:%S}:{1:<03d}] {2} <{3}> {4}", *std::localtime(&cur_time), mils, PRIOR2STR[(uint8_t)level], prefix, message));

            pimpl_->tmp_log_.emplace_back(std::move(print_msg));
            pimpl_->tmp_log_size_++;
            pimpl_->last_printed_data_.prefix = prefix;
            pimpl_->last_printed_data_.level = level;
            pimpl_->last_printed_data_.message = message;
        }
        //pimpl_->async_logger->flush();
    }
    catch (...)
    {
    }

}

void MTLogger::flushLog()
{
    pimpl_->file_.flush();
}

void MTLogger::updateParams(const std::string & file_name_base, const std::string & log_dir, size_t max_file_size)
{
    if (!file_name_base.empty())
    {
        pimpl_->file_name_base_ = file_name_base;
        auto dot_pos = file_name_base.rfind(".");
        if (dot_pos != std::string::npos)
        {
            pimpl_->file_ext_ = pimpl_->file_name_base_.substr(dot_pos + 1);
            if (pimpl_->file_ext_.length() > 3)
                pimpl_->file_ext_ = "log";
            else
                pimpl_->file_name_base_ = pimpl_->file_name_base_.substr(0, dot_pos);
        }
    }

    if (!log_dir.empty())
    {
        pimpl_->log_dir_ = log_dir;
        if (pimpl_->log_dir_.empty())
            pimpl_->log_dir_ = ".\\";
        else if (pimpl_->log_dir_.back() != '\\')
            pimpl_->log_dir_.append("\\");
    }

    if (max_file_size)
    {
        pimpl_->max_file_size_ = max_file_size;
    }
}

void MTLogger::run(bool & terminated)
{
    uint32_t sleep_time = 500;
    while (!terminated)
    {
        if (!pimpl_->tmp_log_size_)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        else
        {
            {
                std::lock_guard <spinlock_mutex> lock_(pimpl_->logger_mut_);
                pimpl_->work_log_.swap(pimpl_->tmp_log_);
                pimpl_->tmp_log_size_ = 0;
            }
            for (const auto& str : pimpl_->work_log_)
            {
                pimpl_->writeToFile(str);
            }
            flushLog();
            pimpl_->work_log_.clear();
        }
    }

    {
        std::lock_guard <spinlock_mutex> lock_(pimpl_->logger_mut_);
        pimpl_->work_log_.swap(pimpl_->tmp_log_);
        pimpl_->tmp_log_size_ = 0;
    }
    for (const auto& str : pimpl_->work_log_)
    {
        pimpl_->writeToFile(str);
    }
    flushLog();
    pimpl_->work_log_.clear();
    terminated = true;
}

