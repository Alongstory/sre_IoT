#include <string.h>
#include <stdbool.h>
#include <stdarg.h>


#ifndef SRC_IOTHUBAGENT_LOGGER_H_
#define SRC_IOTHUBAGENT_LOGGER_H_

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


/**
 * @ingroup logging
 * Logger level
 */
typedef enum {
  IOTHUBAGENT_LOGGER_LEVEL_DEBUG,		///< DEBUG
  IOTHUBAGENT_LOGGER_LEVEL_INFO,    ///< INFO
  IOTHUBAGENT_LOGGER_LEVEL_ERROR,   ///< ERROR
  IOTHUBAGENT_LOGGER_LEVEL_DISABLED ///< DISABLED
} iothubAgent_loggerLevel_t ;

/**
 * @ingroup logging
 * Callback called every time the agent want to issue a log trace
 * @param p_lvl log level of the trace
 * @param p_fileName the file name from which the log comes
 * @param p_line the line from which the log was issued
 * @param p_logString the log string it self
 */
typedef void (*iothubAgent_logger_logEventCbk)(iothubAgent_loggerLevel_t p_lvl, const char* p_fileName, int p_line, const char* p_logString);

/**
 * @ingroup logging
 * Logger structure
 */
typedef struct iothubAgent_logger {
	const char* name;                 ///< Name of the logger
	iothubAgent_loggerLevel_t level;  ///< Current level of the logger
} iothubAgent_logger_t;

/**
 * @ingroup logging
 * Declare a new Logger
 * @param logger The Logger_t variable name.
 * @param name const char* containing the logger name to log (must be persistent).
 * @param level The initial level to set.
 */
#define IOTHUBAGENT_LOGGER_DECLARE(logger,name,level) iothubAgent_logger_t logger={name,level}

/**
 * @ingroup logging
 * Export an existing logger
 */
#define IOTHUBAGENT_LOGGER_EXPORT(logger) extern iothubAgent_logger_t logger


/**
 * @ingroup logging
 * Set the log event callback to get logs instead of printing them on stdout
 * @param logEventCbk the log event callback
 */
#define IOTHUBAGENT_LOGGER_SET_LOG_EVENT_CBK(logEventCbk) _iothubAgent_logger_setLogEventCallBack(logEventCbk)

/**
 * @ingroup logging
 * Return the logger name
 * @param logger The logger declared with LOGGER_DECLARE
 * @return The logger name
 */
#define IOTHUBAGENT_LOGGER_GET_NAME(logger) _iothubAgent_logger_getLoggerName(logger)

/**
 * Change logger level
 * @param logger The logger declared with LOGGER_DECLARE
 * @param level The level to set.
 */
#define IOTHUBAGENT_LOGGER_SET_LEVEL(logger, level) _iothubAgent_logger_setLoggerLevel(&logger, level)

/**
 * @ingroup logging
 * Get logger level
 * @param logger The logger declared with LOGGER_DECLARE
 * @return the logger level
 */
#define IOTHUBAGENT_LOGGER_GET_LEVEL(logger) _iothubAgent_logger_getLoggerLevel(logger)

/**
 * @ingroup logging
 * Get the logger level name
 * @param level The level to get the name of.
 * @return A null terminated string containing the corresponding logger level name
 */
#define IOTHUBAGENT_LOGGER_GET_LEVEL_NAME(level) _iothubAgent_loggerLevelToString(level)

/**
 * Enable colored logs
 */
#define IOTHUBAGENT_LOGGER_DISABLE_COLOR _iothubAgent_loggerSetColor(false)

/**
 * @ingroup logging
 * Disable colored logs
 */
#define IOTHUBAGENT_LOGGER_ENABLE_COLOR _iothubAgent_loggerSetColor(true)


#define IOTHUBAGENT_LOG_DEBUG(logger, ...)  _iothubAgent_log(logger, IOTHUBAGENT_LOGGER_LEVEL_DEBUG, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

#define IOTHUBAGENT_LOG_INFO(logger, ...)   _iothubAgent_log(logger, IOTHUBAGENT_LOGGER_LEVEL_INFO, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

#define IOTHUBAGENT_LOG_ERROR(logger, ...)  _iothubAgent_log(logger, IOTHUBAGENT_LOGGER_LEVEL_ERROR, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)


#ifdef  __cplusplus
extern "C" {
#endif

/**
 * Set the log event callback to get logs instead of printing them on stdout,
 * do not use it directly, use IOTHUBAGENT_LOGGER_SET_LOG_EVENT_CBK instead
 * @param p_logEventCbk the log event callback
 */
void _iothubAgent_logger_setLogEventCallBack(iothubAgent_logger_logEventCbk p_logEventCbk);

/**
 * Set the logger level at run time, do not use it directly, use IOTHUBAGENT_LOGGER_SET_LEVEL instead
 * @param p_logger The pointer to the logger.
 * @param p_level The new level to set.
 */
void _iothubAgent_logger_setLoggerLevel(iothubAgent_logger_t *p_logger, iothubAgent_loggerLevel_t p_level);

/**
 * Get the logger level at run time, do not use it directly, use IOTHUBAGENT_LOGGER_GET_LEVEL instead
 * @param p_logger The logger.
 * @return The logger level.
 */
iothubAgent_loggerLevel_t _iothubAgent_logger_getLoggerLevel(iothubAgent_logger_t p_logger);

/**
 * This is the logging function, do not use it directly, use IOTHUBAGENT_LOG_XXX(...) instead
 * @param p_logger The logger of the trace.
 * @param p_lvl The log level of the trace.
 * @param p_file File from which the trace comes.
 * @param p_line Line where the trace is present.
 * @param p_function Function in which the trace is located.
 */
void _iothubAgent_log(iothubAgent_logger_t p_logger, iothubAgent_loggerLevel_t p_lvl, const char* p_file, const int p_line, const char* p_function, ...);

/**
 * This is the variadic logging function, do not use it directly, use IOTHUBAGENT_LOG_XXX(...) instead
 * @param p_logger The logger of the trace.
 * @param p_lvl The log level of the trace.
 * @param p_file File from which the trace comes.
 * @param p_line Line where the trace is present.
 * @param p_function Function in which the trace is located.
 * @param p_format Formatted string to be printed
 * @param p_args Variadic list of the arguments for the formatted string (undefined after call)
 */
void _iothubAgent_vlog(iothubAgent_logger_t p_logger, iothubAgent_loggerLevel_t p_lvl, const char* p_file, int p_line, const char* p_function, const char* p_format, va_list p_args);


/**
 * Return the logger name,  do not use it directly, use IOTHUBAGENT_LOGGER_GET_NAME(...) instead
 * @param p_logger The logger
 * @return The logger name
 */
const char* _iothubAgent_logger_getLoggerName(iothubAgent_logger_t p_logger);

/**
 * Get the logger level name, do not use it directly, use IOTHUBAGENT_LOGGER_GET_LEVEL_NAME(...) instead
 * @param p_level The level to get the name of.
 * @return A null terminated string containing the corresponding logger level name
 */
const char* _iothubAgent_loggerLevelToString(iothubAgent_loggerLevel_t p_level);

/**
 * Enable or Disable color in logs, do not use it directly, use LOGGER_ENABLE_COLOR or LOGGER_DISABLE_COLOR instead
 * @param p_setColor Set to true to enable color, set to false otherwise
 */
void _iothubAgent_loggerSetColor(bool p_setColor);


#ifdef  __cplusplus
}
#endif

#endif /* SRC_IOTHUBAGENT_LOGGER_H_ */
