#include <iostream>
#include "friend.pb.h"
#include "mprpcapplication.h"

int main(int argc, char ** argv)
{
    // 初始化
    MprpcApplication::Init(argc, argv);

    // 调用远程rpc方法代理类
    fixbug::FriendServiceRpc_Stub stub(new MpRpcChannel());

    // 业务
    fixbug::GetFriendListRequest request;
    fixbug::GetFriendListResponse response;
    request.set_id(999);

    // 通过代理调用方法 同步调用
    MprpcController controller;
    stub.GetFriendList(&controller, &request, &response, nullptr);
    // stub.Login(nullptr, &request, &response, nullptr);
    
    // 调用完成
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc GetFriendList response : " << response.success() << std::endl;
            uint32_t friend_size = response.friend__size();
            for (int i = 0; i < friend_size; ++i)
            {
                std::cout << "friends : " << (i + 1) << response.friend_(i) << std::endl;; 
            }
        }
        else
        {
            std::cout << "rpc GetFriendList response error : " << response.result().errmsg() << std::endl;
        }
    }

    std::cout << "=============== caller is over ===============" << std::endl;

    return 0;
}