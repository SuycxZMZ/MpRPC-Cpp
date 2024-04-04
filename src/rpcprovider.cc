#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"

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

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        conn->shutdown();
    }
}

// 处理远程调用请求
/*
service_name method_name args 
单个请求帧格式 header_size + header(service_name method_name) + args_size + args
args_size 可以防止tcp粘包。
*/
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, 
                            muduo::net::Buffer * buffer, 
                            muduo::Timestamp time)
{
    std::string recv_buf = buffer->retrieveAllAsString();

    // 读取头部大小
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    // 读取头部原始字符流
    std::string header_str = recv_buf.substr(4, header_size);

    // 反序列化
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(header_str))
    {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else
    {
        // 反序列化失败
        std::cout << "rpcHeader header_str : " << header_str << " pase error !!!" << std::endl;
        return; 
    }
    // 取出参数
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // 调试信息
    std::cout << "=======================================" << std::endl;
    std::cout << "header_size : " << header_size << std::endl;
    std::cout << "service_name : " << service_name << std::endl;
    std::cout << "method_name : " << method_name << std::endl;
    std::cout << "args_size : " << args_size << std::endl;
    std::cout << "args_str : " << args_str << std::endl;
    std::cout << "=======================================" << std::endl;
}