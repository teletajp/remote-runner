#pragma once
#include <string>
namespace rrd
{

/** @class IProcessor
 *  @brief Интерфейс процессора
 */
class IProcessor
{
public:
    enum class Result
    {
        success = 0,
    };
    /**
    * @brief Обработать команду
    * @param [in] command команда
    */
    virtual Result process(const std::string &command) = 0;
    /**
    * @brief Вернуть результат
    * @param [in] result результат
    */
    virtual Result complite(const std::string &result) = 0;
};
}