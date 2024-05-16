#ifndef LOGGER_H
#define LOGGER_H

#include "lockqueue.h"
#include <string>
#include <thread>

enum LogLevel
{
    INFO,
    ERROR,
};

// 日志模块 设计为单例
class Logger
{
public:
    // 获取logger单例
    static Logger &Getinstance();

    // 日志级别
    void SetLogLevel(LogLevel level);

    // 写日志
    void Log(std::string msg);

private:
    int m_loglevel;

    LockQueue<std::string> m_lockqueue;

    Logger();

    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
};

#define LOG_INFO(logmsgformat, ...)                     \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::Getinstance();         \
        logger.SetLogLevel(INFO);                       \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c);                                  \
    } while (0);

#define LOG_ERROR(logmsgformat, ...)                    \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::Getinstance();         \
        logger.SetLogLevel(ERROR);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c);                                  \
    } while (0);

#endif