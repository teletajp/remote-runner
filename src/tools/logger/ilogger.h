#pragma once
#include <string>
namespace logger{
  enum class PriorityLevel : uint8_t {
    FATAL = 0,          //0 (Fatal)система не работоспособна
    ALERT = 1,          //1 (Alert)система требует немедленного вмешательства
    CRIT = 2,           //2 (Critical)состояние системы критическое
    ERRO = 3,           //3 (Error)сообщения об ошибках
    WARN = 4,           //4 (Warning)предупреждения о возможных проблемах
    NOTICE = 5,         //5 (Notice)сообщения о нормальных, но важных событиях
    INFO = 6,           //6 (Informational)информационные сообщения
    DEBG = 7,           //7 (Debug)отладочные сообщения
    MAX_LVL = DEBG,     // Максимальный уровень
  } ;

#define PRIORITY_LEVEL_COUNTS  (8)

#define INLOG(prior, ...) Logger ? Logger->Write(prior, __VA_ARGS__) : (printf("%s ", PRIOR2STR(prior)) && printf(__VA_ARGS__) && printf("\r\n"))
  
/**
* @class ILogger
* @brief Класс интерфейс логгера
*/
class ILogger{
public:
  ILogger()
  {
      PRIOR2STR[uint8_t(PriorityLevel::FATAL)] = "FATAL";
      PRIOR2STR[uint8_t(PriorityLevel::ALERT)] = "ALERT";
      PRIOR2STR[uint8_t(PriorityLevel::CRIT)] = "CRIT";
      PRIOR2STR[uint8_t(PriorityLevel::ERRO)] = "ERROR";
      PRIOR2STR[uint8_t(PriorityLevel::WARN)] = "WARN";
      PRIOR2STR[uint8_t(PriorityLevel::NOTICE)] = "NOTICE";
      PRIOR2STR[uint8_t(PriorityLevel::INFO)] = "INFO";
      PRIOR2STR[uint8_t(PriorityLevel::DEBG)] = "DEBG";
  };
  virtual ~ILogger()
  {
  };

  virtual void printLog(const std::string &prefix, PriorityLevel level, const std::string &message) = 0;
  virtual void flushLog() = 0;

  const char* PRIOR2STR[PRIORITY_LEVEL_COUNTS];
};

} //namespace logger
typedef std::shared_ptr<logger::ILogger> logget_ptr_t;
using PL = logger::PriorityLevel;