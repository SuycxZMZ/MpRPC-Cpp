#ifndef PROVIDER_H
#define PROVIDER_H

#include "google/protobuf/service.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <google/protobuf/descriptor.h>
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

    // Service 服务类型信息
    struct ServiceInfo
    {
        // 服务对象
        google::protobuf::Service * m_service;
        // 服务方法映射表
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap;
    };
    
    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceInfoMap;

    muduo::net::EventLoop m_eventLoop; 

    // 事件回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);

    // 消息回调
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);

    // Closure回调
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message *);
};


#endif