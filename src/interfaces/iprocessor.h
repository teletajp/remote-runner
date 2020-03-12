#pragma once
#include <string>
namespace rrd
{

/** @class IProcessor
 *  @brief ��������� ����������
 */
class IProcessor
{
public:
    enum class Result
    {
        success = 0,
    };
    /**
    * @brief ���������� �������
    * @param [in] command �������
    */
    virtual Result process(const std::string &command) = 0;
    /**
    * @brief ������� ���������
    * @param [in] result ���������
    */
    virtual Result complite(const std::string &result) = 0;
};
}