#ifndef PROVIDER_H
#define PROVIDER_H

#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

class RpcProvider
{
public:
    // 框架提供给外部使用的，可以发布rpc调用的接口
    void NotifyService(google::protobuf::Service * service);
    // 启动rpc服务节点，提供远程rpc调用服务
    void Run();
private:
    muduo::net::EventLoop m_eventLoop; 

    // 事件回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);

    // 消息回调
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
};


#endif