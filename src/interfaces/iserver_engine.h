#pragma once
#include <delegate.h>
namespace rrd
{
template<class Owner>
using RCallBack = Delegate<Owner, const int /*fd*/>;

template<class Owner>
using WCallBack = Delegate<Owner, const int /*fd*/>;

template<class Owner>
using ECallBack = Delegate<Owner, const int /*fd*/, const std::string &/*error_message*/>;
/** @class IServerEngine
 *  @brief ��������� ��� ������ �������
 */
template<class Owner>
class IServerEngine
{
public:
   /**
   * @brief ��������� ������� ��� ������� ������
   * @param [in] rcb ������, ������������ ����� �������
   * 
   * ������ ������ ������� ������ �� ������ � �������� ��� ������ ��� ��� ������� ��������� ������ � ������
   */
    virtual void setReadCb(const RCallBack<Owner> rcb) = 0;
    /**
    * @brief ��������� ������� ��� ������� ������
    * @param [in] wcb ������, ������������ ����� �������
    *
    * ������ ������ ������� ������ �� ������ � �������� ��� ������ ��� ��� ������� �������� ������ � ������
    */
    virtual void setWriteCb(const WCallBack<Owner> wcb) = 0;
    /**
    * @brief ��������� ������� ��� ������� ������
    * @param [in] ecb ������, ������������ ����� �������
    *
    * ������ ������ ������� ������ �� ������ � �������� ��� ������ ��� ��� ������� ��������� ������ � ������
    */
    virtual void setErrorCb(const ECallBack<Owner> ecb) = 0;
    /**
    * @brief �������� ������ �����
    *
    * � �������� ����� ���������� �������� ������������ ������� � ����� ��������������� ������� 
    */
    virtual bool nextLoop() = 0;
    /**
    * @brief ���������� ����������� ������ � ��������� �������������� ������������
    */
    virtual bool addSocket(int fd) = 0;
    /**
    * @brief �������� ����������� ������ �� ��������� �������������� ������������
    */
    virtual bool delSocket(int fd) = 0;
};
}