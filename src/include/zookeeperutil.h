#ifndef ZOOKEEPERUTIL_H
#define ZOOKEEPERUTIL_H

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // 启动连接 zkserver
    void start();
    // 在 zkserver 上根据path创建节点
    void create(const char * path, const char * data, int datalen, int state = 0);
    std::string GetData(const char * path);
private:
    // zk 客户端句柄
    zhandle_t * m_zhandle;
};

#endif