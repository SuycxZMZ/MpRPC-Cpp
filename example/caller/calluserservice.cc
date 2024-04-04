#include <iostream>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"


int main(int argc, char ** argv)
{
    // 初始化
    MprpcApplication::Init(argc, argv);

    // 调用远程rpc方法代理类
    fixbug::UserServiceRpc_Stub stub(new MpRpcChannel());

    // 业务
    fixbug::LoginRequest request;
    fixbug::LoginResponse response;
    request.set_name("zhang san");
    request.set_pwd("123456");

    // 通过代理调用方法 同步调用
    stub.Login(nullptr, &request, &response, nullptr);

    // 调用完成
    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response : " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }

    return 0;
}