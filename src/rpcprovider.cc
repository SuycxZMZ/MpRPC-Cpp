#include "rpcprovider.h"
#include "mprpcapplication.h"

void RpcProvider::NotifyService(google::protobuf::Service * service)
{
    ServiceInfo service_info;

    const google::protobuf::ServiceDescriptor * serviceDesc = service->GetDescriptor();
    // 获取服务名字
    const std::string serviceName = serviceDesc->name();
    // 获取服务方法的数量
    int methodCnt = serviceDesc->method_count();

    for (int i = 0; i < methodCnt; ++i)
    {
        // 获取服务对象指定方法的描述
        const google::protobuf::MethodDescriptor * methodPtr = serviceDesc->method(i);
        std::string method_name = methodPtr->name();
        service_info.m_methodMap.emplace(method_name, methodPtr);
    }
    service_info.m_service = service;
    m_serviceInfoMap.emplace(serviceName, service_info);
}

// 启动 muduo server
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    // 创建 TcpServer
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcServer");

    // 绑定事件回调
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置server线程数量
    server.setThreadNum(4);

    // 启动server
    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr&)
{

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp)
{

}