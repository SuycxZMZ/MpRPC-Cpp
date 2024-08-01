#include "zookeeperutil.h"
#include "mprpcapplication.h"

/**
 * @SuycxZMZ
 * @brief 类型为 watcher_fn 全局观察器
 * @details zookeeper 服务器端状态改变回调，这里rpcprovider也是zookeeper客户端，需要使用watcher回调
 *          这里仅用了与服务端会话完成连接的回调，完成连接时唤醒 ZkClient::start() 线程返回
 *          经常说的服务发现与治理，主要就是处理一系列zkserver上发生的事情，zkclient负责对不同的事件进行处理，修改自身存储的信息
 * @param zh ZooKeeper 客户端的句柄，即 zhandle_t 类型的指针，表示与 ZooKeeper 服务器的连接。
 * @param type 事件类型，可以是以下值之一：
 *          ZOO_SESSION_EVENT：会话事件，表示与 ZooKeeper 服务器的连接状态发生变化。
 *          ZOO_CREATED_EVENT：节点创建事件。
 *          ZOO_DELETED_EVENT：节点删除事件。
 *          ZOO_CHANGED_EVENT：节点数据变化事件。
 *          ZOO_CHILD_EVENT：子节点变化事件。
 * @param state 事件状态，可以是以下值之一：
 *          ZOO_EXPIRED_SESSION_STATE：会话过期状态。
 *          ZOO_AUTH_FAILED_STATE：认证失败状态。
 *          ZOO_CONNECTING_STATE：连接中状态。
 *          ZOO_ASSOCIATING_STATE：关联中状态。
 *          ZOO_CONNECTED_STATE：已连接状态。
 *          ZOO_CONNECTEDREADONLY_STATE：只读已连接状态。
 * @param path 事件路径，表示发生事件的节点路径。
 * @param watcherCtx 观察者上下文，用于传递额外的信息。
*/
void global_watcher(zhandle_t * zh, int type, 
                    int state, const char * path, void * watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)
    {
        if (state == ZOO_CONNECTED_STATE)
        {
            sem_t * sem = (sem_t *)zoo_get_context(zh);
            sem_post(sem); 
        }
    }
}

ZkClient::ZkClient() : m_zhandle(nullptr)
{

}

ZkClient::~ZkClient()
{
    if (nullptr != m_zhandle)
    {
        zookeeper_close(m_zhandle);
    }
}

void ZkClient::start()
{
    std::string zookeeperip = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string zookeeperport= MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string conn_str = zookeeperip + ":" + zookeeperport;

    /**
     * 非阻塞直接返回
     * 多线程版的API: 
     * API调用线程
     * 网络线程
     * global_watcher 回调线程
     * zookeeper_init 直接返回不阻塞，先设置句柄
    */
    m_zhandle = zookeeper_init(conn_str.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle)
    {
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    // 设置句柄信息 用于在 m_zhandle 上等待
    // 回调线程负责 post
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
}

void ZkClient::create(const char * path, const char * data, int datalen, int state)
{
    char path_buffer[128];
    int buff_len = sizeof(path_buffer);
    int flag = 0;
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (ZNONODE == flag)
    {
        flag = zoo_create(m_zhandle, path, data, datalen, 
                          &ZOO_OPEN_ACL_UNSAFE, 
                          state, path_buffer, buff_len);
        if (ZOK == flag)
        {
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char * path)
{
    char buffer[128];
    int buffer_len = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &buffer_len, nullptr);

    if (ZOK != flag)
    {
        return "";
    }
    else
    {
        return std::string(buffer);
    }
}