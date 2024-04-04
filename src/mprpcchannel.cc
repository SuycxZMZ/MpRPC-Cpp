#include "mprpcchannel.h"
#include "mprpcapplication.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <error.h>


void MpRpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, 
                        const google::protobuf::Message* request,
                        google::protobuf::Message* response, 
                        google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* service_desc = method->service();
    std::string service_name = service_desc->name();
    std::string method_name = method->name();

    uint32_t args_size = 0;
    std::string args_str;
    if (!request->SerializeToString(&args_str))
    {
        std::cout << "Serialize request args_str error !!!" << std::endl;
        return;
    }
    args_size = args_str.size();

    // set rpcheader
    mprpc::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);
    rpcheader.set_method_name(method_name);
    rpcheader.set_args_size(args_size);

    // 序列化 rpcheader
    std::string rpcheader_str;
    if (!rpcheader.SerializeToString(&rpcheader_str))
    {
        std::cout << "Serialize request rpcheader error !!!" << std::endl;
        return;
    }
    uint32_t rpcheader_size = rpcheader_str.size();

    // 组装 rpc 请求帧
    std::string rpc_send_str;
    // rpcheader_size
    rpc_send_str.insert(0, std::string((char*)&rpcheader_size, 4));
    // rpcheader
    rpc_send_str += rpcheader_str;
    // args
    rpc_send_str += args_str;

    // Debug info
    std::cout << "=======================================" << std::endl;
    std::cout << "header_size : " << rpcheader_size << std::endl;
    std::cout << "service_name : " << service_name << std::endl;
    std::cout << "method_name : " << method_name << std::endl;
    std::cout << "args_size : " << args_size << std::endl;
    std::cout << "args_str : " << args_str << std::endl;
    std::cout << "=======================================" << std::endl;

    // 已经打包好序列化字符流 开始通过网络发送
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd) 
    {
        std::cout << "error create socket : " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 连接
    if (-1 == connect(clientfd, (sockaddr*)&server_addr, sizeof(server_addr)))
    {
        std::cout << "connect error : " << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    // 发送
    if (-1 == send(clientfd, rpc_send_str.c_str(), rpc_send_str.size(), 0))
    {
        std::cout << "send error : " << errno << std::endl;
        close(clientfd);
        // exit(EXIT_FAILURE); 
        return;       
    }

    // 接收响应
    char recv_buf[1024] = {0};
    uint32_t recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        std::cout << "recv error : " << errno << std::endl;
        close(clientfd);
        // exit(EXIT_FAILURE); 
        return;
    }

    // 反序列化 response
    std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromString(response_str))
    {
        std::cout << "parse response error !!! " << std::endl;
        close(clientfd);
        return;
    }

    close(clientfd);
}