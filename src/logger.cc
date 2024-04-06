#include "logger.h"
#include <time.h>
#include <iostream>

// 构造
Logger::Logger()
{
    std::thread writeLogTask([&]()
    {
        for (;;)
        {
            // 获取日期 获取内容 写
            time_t now = time(nullptr);
            tm * nowtime = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", nowtime->tm_year+1900, nowtime->tm_mon+1, nowtime->tm_mday);
            FILE * fp = fopen(file_name, "a+");
            if (nullptr == fp)
            {
                std::cout << "open file " << file_name << " error" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lockqueue.pop();

            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d [%s] : ", 
                    nowtime->tm_hour, 
                    nowtime->tm_min, 
                    nowtime->tm_sec,
                    (m_loglevel == INFO) ? "INFO" : "ERROR");
            msg.insert(0, time_buf);
            msg.append("\n");

            fputs(msg.c_str(), fp);
            fclose(fp);
        }
    });
    writeLogTask.detach();
}

// 获取logger单例
Logger& Logger::Getinstance()
{
    static Logger logger;
    return logger;
}

// 日志级别
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

// 写日志
void Logger::Log(std::string msg)
{
    m_lockqueue.push(msg);
}