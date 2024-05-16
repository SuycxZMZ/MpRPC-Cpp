#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include "logger.h"

// 全局 watcher 观察器   server给client的通知
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

    /*
    异步 
    多线程版的API: 
    API调用线程
    网络IO线程
    global_watcher 回调线程
    zookeeper_init 直接返回不阻塞，先设置句柄
    */  
    m_zhandle = zookeeper_init(conn_str.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle)
    {
        LOG_ERROR("in ZkClient::start() : zookeeper_init error");
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    // 设置句柄信息 用于在 m_zhandle 上等待
    // 回调线程负责 post
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    LOG_INFO("ZkClient init success");
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
            LOG_INFO("znode create success. path : %s", path);
        }
        else
        {
            LOG_ERROR("znode create error. path : %s, flag : %d", path, flag);
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
        LOG_ERROR("get znode error path : %s", path);
        return "";
    }
    else
    {
        return std::string(buffer);
    }
}