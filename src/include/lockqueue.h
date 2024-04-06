#ifndef LOCKQUEUE_H
#define LOCKQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// 异步写日志队列
template <typename T>
class LockQueue
{
public:
    // 作为生产者，唤醒等待的写日志线程
    void push(const T & data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_convariable.notify_one();
    }
    // 作为消费者，等待被唤醒
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        // 队列为空则释放锁
        while (m_queue.empty())
        {
            m_convariable.wait(lock);
        }

        T data = m_queue.front();
        m_queue.pop();
        return data;
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_convariable;
};


#endif